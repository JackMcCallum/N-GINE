#ifndef __NGINE_DIRECTORY_H_
#define __NGINE_DIRECTORY_H_

#include "NGineCommon.h"

namespace NGine
{
	class Directory
	{
	public:
		class File
		{
			friend class Directory;

		public:
			std::string fileName;
			std::string filePath;
			uint32 sizeInBytes;
			uint32 lastWrite;
			std::vector<File*> children;

			// True if this file is a directory
			bool isDirectory : 1;
			// True if the directory was rescanned and the last write was diferent
			bool wasModified : 1;
			// True if the directory was rescanned and this file was not there before.
			bool isNew : 1;
			// When scanning, files are set to non existent until proven wrong again
			bool exists : 1;
			// True only on the first file, which is the root file
			bool isRoot : 1;

			void recursivly(std::function<bool(File* file)> function);

		protected:
			File();
			~File();

			void _scanDirectory(bool recurse = true);
			void _processFile(WIN32_FIND_DATA& findData, bool recurse);
			void _markAsNonexistant();
		};

	public:
		Directory(const std::string& rootDirectory);
		bool scanDirectory();
		File* getRootFile();

	private:
		File mRootFile;
		bool mIsValid;
	};
}

#endif // !__NGINE_DIRECTORY_H_
