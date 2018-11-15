#pragma once

#include "EntryDataFormat.h"
#include "Utility/PropertyList/PropertyList.h"
class ArchiveEntry;

class EntryType
{
public:
	EntryType(string id = "Unknown");
	~EntryType();

	// Getters
	const string& id() const { return id_; }
	const string& name() const { return name_; }
	const string& extension() const { return extension_; }
	const string& formatId() const { return format_->getId(); }
	const string& editor() const { return editor_; }
	const string& category() const { return category_; }
	const string& icon() const { return icon_; }
	int           index() const { return index_; }
	uint8_t       reliability() const { return reliability_; }
	PropertyList& extraProps() { return extra_; }
	rgba_t        colour() const { return colour_; }

	// Misc
	void   addToList();
	void   dump();
	void   copyToType(EntryType* target);
	string fileFilterString();

	// Magic goes here
	int isThisType(ArchiveEntry* entry);

	// Static functions
	static bool               readEntryTypeDefinition(MemChunk& mc, const string& source);
	static bool               loadEntryTypes();
	static bool               detectEntryType(ArchiveEntry* entry);
	static EntryType*         fromId(const string& id);
	static EntryType*         unknownType();
	static EntryType*         folderType();
	static EntryType*         mapMarkerType();
	static wxArrayString      iconList();
	static void               cleanupEntryTypes();
	static vector<EntryType*> allTypes();
	static vector<string>     allCategories();

private:
	// Type info
	string  id_;
	string  name_;
	string  extension_; // File extension to use when exporting entries of this type
	string  icon_;      // Icon to use in entry list
	string  editor_;    // The in-program editor to use (hardcoded ids, see *EntryPanel constructors)
	string  category_;  // The type 'category', used for type filtering
	int     index_;
	rgba_t  colour_;
	bool    detectable_;  // False only for special types that should be set not detected
	uint8_t reliability_; // How 'reliable' this type's detection is. A higher value means it's less likely this
						  // type can be detected erroneously. 0-255 (default is 255)
	PropertyList extra_;  // Extra properties for types with certain special behaviours
						  // Current recognised properties listed below:
						  // bool "image": Can be loaded into a SImage, therefore can be converted to other
						  // supported image formats
						  // bool "patch": Can be used as a TEXTUREx patch
						  // string "image_format": An SIFormat type id 'hint', mostly used for
						  // Misc::loadImageFromEntry

	// Type matching criteria
	EntryDataFormat* format_;            // To be of this type, the entry data must match the specified format
	bool             match_ext_or_name_; // If true, the type fits if it matches the name OR the extension, rather
										 // than having to match both. Many definition lumps need this.
	vector<string> match_extension_;
	vector<string> match_name_;
	vector<int>    match_size_;
	int            size_limit_[2]; // Minimum/maximum size
	vector<int>    size_multiple_; // Entry size must be a multiple of this
	vector<string> section_;       // The 'section' of the archive the entry must be in, eg "sprites" for entries
								   // between SS_START/SS_END in a wad, or the 'sprites' folder in a zip
	vector<string> match_archive_; // The types of archive the entry can be found in (e.g., wad or zip)
};
