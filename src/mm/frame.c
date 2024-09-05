#include "mm/frame.h"
#include "mm/memory.h"
#include "status.h"
#include "helper/errors.h"

static int validate_table(void* first, void* end, struct frame_table* table)
{
	int res = 0;

	size_t table_size = (size_t)(end - first);
	size_t total_frames = table_size / FRAME_SIZE;
	if (table->total != total_frames)
	{
		res = -EINVARG;
		panic("table size is not correct");
		goto out;
	}

out:
	return res;
}

bool frames_validate_alignment(void* frame_ptr)
{
	return ((unsigned int)frame_ptr % FRAME_SIZE) == 0;
}

int kernel_frames_create(struct frames* frames, void* first_frame, void* last_frame, struct frame_table* table)
{
	int res = 0;
	if (!frames_validate_alignment(first_frame) || !frames_validate_alignment(last_frame))
	{
		res = -EINVARG;
		panic("frame table is not aligned");
		goto out;
	}
	memset(frames, 0, sizeof(struct frames));
	frames->saddr = first_frame;
	frames->table = table;

	res = validate_table(first_frame, last_frame, table);
	if (res < 0)
	{
		goto out;
	}

	size_t table_size = sizeof(FRAME_TABLE_ENTRY) * table->total;
	memset(table->entries, FRAME_IS_FREE,  table_size);

out:
	return res;
}

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
	for (int i = 0; i < table->total; i++)
	{
		if (frame_get_entry_type(table->entries[i]) != FRAME_IS_FREE)
		{
			fc = 0;
			fs = -1;
			continue;	
		}

		// if thisi s first frame we found
		if (fs == -1)
		{
			fs = i;
		}
		fc++;
		if (fc == total_frames)
		{
			break;
		}
	}
	if (fs == -1)
	{
		panic("fs count in get_start_frame is still -1");
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
	int total_frames = aligned_size / FRAME_SIZE;

	return frame_alloc(frames, total_frames);
}

int address_to_frame(struct frames* frames, void* address)
{
	return ((int)(address - frames->saddr) / FRAME_SIZE);
}

void mark_frames_as_free(struct frames* frames, int starting_frame)
{
	struct frame_table* table = frames->table;
	for (int i = starting_frame; i < (int)table->total; i++)
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
