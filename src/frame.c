#include "frame.h"
#include "status.h"

struct frames* kernel_frames;
struct frame_table* kernel_frame_table;


uint32_t align_frame_size_to_upper(uint32_t size)
{
	if ((size % FRAME_SIZE) == 0)
	{
		return size;
	}	

	size = (size - (size % FRAME_SIZE));
	size += FRAME_SIZE;
	return size;
}

static int frame_get_entry_type(FRAME_TABLE_ENTRY entry)
{
	return entry & 0x0f;
}

int get_start_frame(struct frames* frames, int total_frames)
{
	struct frame_table* table = frames->table;
	int fc = 0;
	int fs = -1;
	for (int i = 0; i < frames->total; i++)
	{
		if (frame_get_entry_type(table->entries[i]) != FRAME_IS_FREE)
		{
			fc = 0;
			fs = -1;
			continue;	
		}

		// if thisi s first frame we found
		if (fs = -1)
		{
			fc = i;
		}
		fc++;
		if (fc == total_frames)
		{
			break;
		}
	}
	if (fs == -1)
	{
		return -ENOMEM;
	}

	return fs;
}

void* frame_to_address(struct frames* frames, int start_frame)
{
	return (frames->saddr + (start_frame * FRAME_SIZE));
}

void mark_frames_as_taken(struct frames* frames, int start_frame, int total_frames)
{
	int last_frame = (start_frame + total_frames) - 1;
	FRAME_TABLE_ENTRY entry = FRAME_IS_TAKEN | FRAME_IS_FIRST;
	if (total_frames > 1)
	{
		entry |= FRAME_HAS_NEXT;
	}

	for (int i = start_frame; i <= last_frame; i++)
	{
		frames->table->entries[i] = entry;
		entry = FRAME_IS_TAKEN;
		if (i != last_frame)
		{
			entry |= FRAME_HAS_NEXT;
		}
	}
}

void* frame_alloc(struct frames* frames, int total_frames)
{
	void* address = 0;

	int start_frame = get_start_frame(frames, total_frames);
	if (start_frame < 0)
	{
		goto out;
	}

	address = frame_to_address(frames, start_frame);

	mark_frames_as_taken(frames, start_frame, total_frames);
out:
	return address;
}

void* kernel_frame_alloc(struct frames* frames, size_t size)
{
	int aligned_size = align_frame_size_to_upper(size);
	total_frames = aligned_size / FRAME_SIZE;

	return frame_alloc(frames, total_frames);
}

int address_to_frame(struct frames* frames, void* address)
{
	return ((int)(adress - frames->saddr) / FRAME_SIZE);
}

void mark_frames_as_free(struct frames* frames, int starting_frame)
{
	struct frame_table* table = frames->table;
	for (int i = starting_block; i < (int)table->total; i++)
	{
		FRAME_TABLE_ENTRY entry = table->entries[i];
		table->entries[i] = FRAME_IS_FREE;
		if (!(entry & FRAME_HAS_NEXT))
		{
			break;
		}
	}
}

void kernel_frame_free(struct frames* frames, void* ptr)
{
	mark_frames_as_free(frames, address_to_frame(frames, ptr));
}
