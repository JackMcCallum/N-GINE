#include "NGineStable.h"
#include "NGineDirectory.h"


namespace NGine
{

	Directory::Directory(const std::string& rootDirectory) : mIsValid(false)
	{
		// Ensure that the entered path is infact a directory and not invalid.
		DWORD fAtrib = GetFileAttributes(rootDirectory.c_str());
		if ((fAtrib != INVALID_FILE_ATTRIBUTES) && (fAtrib & FILE_ATTRIBUTE_DIRECTORY))
		{
			mRootFile.isDirectory = true;
			mRootFile.filePath = rootDirectory;
			mRootFile.isRoot = true;
			mRootFile.exists = true;
			mRootFile.isNew = false;

			mIsValid = true;
		}
	}

	bool Directory::scanDirectory()
	{
		if (!mIsValid)
			return false;

		mRootFile._markAsNonexistant();
		mRootFile._scanDirectory();
		return true;
	}

	Directory::File* Directory::getRootFile()
	{
		return &mRootFile;
	}


	Directory::File::File() :
		sizeInBytes(0),
		lastWrite(0),
		isDirectory(false)
	{
	}

	Directory::File::~File()
	{
		for (auto i : children)
			delete i;
	}

	void Directory::File::recursivly(std::function<bool(File* file)> function)
	{
		if (function(this))
		{
			for (auto i : children)
				i->recursivly(function);
		}
	}

	void Directory::File::_scanDirectory(bool recurse)
	{
		if (!isDirectory)
			return;

		WIN32_FIND_DATA findData;
		HANDLE handle;
		std::string filename;

		if (handle = FindFirstFile((filePath + "/*").c_str(), &findData))
		{
			filename = (findData.cFileName);
			if (filename != "." && filename != "..")
				_processFile(findData, recurse);

			while (FindNextFile(handle, &findData))
			{
				filename = (findData.cFileName);
				if (filename != "." && filename != "..")
					_processFile(findData, recurse);
			}
		}
	}

	void Directory::File::_processFile(WIN32_FIND_DATA& findData, bool recurse)
	{
		assert(isDirectory);
		File* fdata = nullptr;

		std::string fname(findData.cFileName);
		auto iter = std::find_if(children.begin(), children.end(), [&](File* file){ return file->fileName == fname; });

		if (iter != children.end())
		{
			// Fild already exists in the tree
			fdata = *iter;
			assert(fdata);

			// Check if it has been modified
			if (fdata->lastWrite != findData.ftLastWriteTime.dwLowDateTime)
				fdata->wasModified = true;

			fdata->lastWrite = findData.ftLastWriteTime.dwLowDateTime;
			fdata->isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			fdata->isRoot = false; // Nope you aren't a root file
			fdata->isNew = false; // File already existed here
			fdata->exists = true; // Proove file exists

		}
		else
		{
			// New file
			fdata = new File();
			assert(fdata);

			fdata->fileName = findData.cFileName;
			fdata->filePath = filePath + "/" + findData.cFileName;
			fdata->sizeInBytes = findData.nFileSizeLow;
			fdata->lastWrite = findData.ftLastWriteTime.dwLowDateTime;
			fdata->isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			fdata->isRoot = false; // Nope not a root file
			fdata->isNew = true; // This is a new file this iteration
			fdata->exists = true; // Yep it exists
			fdata->wasModified = false; // No, its a new file it was not modified

			children.push_back(fdata);
		}

		if (fdata->isDirectory)
			fdata->_scanDirectory(fdata);
	}

	void Directory::File::_markAsNonexistant()
	{
		isNew = false;
		exists = false;
		for (auto i : children)
			i->_markAsNonexistant();
	}

}
