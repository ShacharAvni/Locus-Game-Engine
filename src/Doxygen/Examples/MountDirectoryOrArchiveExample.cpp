//This example shows how to use the Locus FileSystem for
//reading files in archives or on disk.

#include "Locus/FileSystem/FileSystem.h"
#include "Locus/FileSystem/File.h"

#include "Locus/Common/Exception.h"

#include <iostream>

int main(int argc, char** argv)
{
   try
   {
      //This initializes PHYSFS which must be done before other Locus File operations
      Locus::FileSystem fileSystem(argv[0]);

      //Add the directory "path_to/foo". Now MountedFilePaths can be relative to path_to/foo
      Locus::MountDirectoryOrArchive("path_to/foo");

      //Open the file "bar.txt" which must be in the directory path_to/foo.
      Locus::File file1( Locus::MountedFilePath("bar.txt"), Locus::DataStream::OpenMode::Read );

      //Add the archive "path_to/zipFile.zip". Now MountedFilePaths can be relative to path_to/zipFile.zip
      Locus::MountDirectoryOrArchive("path_to/zipFile.zip");

      //Open the file "baz.jpg" which must be either in the directory path_to/foo or in the archive path_to/zipFile.zip.
      Locus::File file2( Locus::MountedFilePath("baz.jpg"), Locus::DataStream::OpenMode::Read );
   }
   catch (Locus::Exception& locusException)
   {
      std::cerr << locusException.Message() << std::endl;
      return 1;
   }
   catch (...)
   {
      std::cerr << "Something unexpected happened." << std::endl;
      return 1;
   }

   return 0;
}