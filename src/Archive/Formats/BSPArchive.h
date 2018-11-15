#pragma once

#include "Archive/Archive.h"

class BSPArchive : public Archive
{
public:
	BSPArchive();
	~BSPArchive();

	// Opening/writing
	bool open(MemChunk& mc) override;                      // Open from MemChunk
	bool write(MemChunk& mc, bool update = true) override; // Write to MemChunk

	// Misc
	bool     loadEntryData(ArchiveEntry* entry) override;
	uint32_t getEntryOffset(ArchiveEntry* entry);

	// Static functions
	static bool isBSPArchive(MemChunk& mc);
	static bool isBSPArchive(string filename);
};
