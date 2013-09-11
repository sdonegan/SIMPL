/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Groeber (US Air Force Research Laboratories
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef MXAFILEINFO_H_
#define MXAFILEINFO_H_

//-- STL includes
#include <string>
//#include <vector>

//-- MXA Includes
#include "EbsdLib/EbsdLib.h"

#define MXAFileInfo EbsdFileInfo

class  MXAFileInfo
{
  public:

    virtual ~MXAFileInfo();
    // -----------------------------------------------------------------------------
    //  These methods are common to both MXADir and MXAFileInfo
    // -----------------------------------------------------------------------------
#if defined (WIN32)
    static EbsdLib_EXPORT const char Separator = '\\';
#else
    static EbsdLib_EXPORT const char Separator = '/';
#endif
    static EbsdLib_EXPORT const char UnixSeparator = '/';
    static EbsdLib_EXPORT const char Dot = '.';


    static EbsdLib_EXPORT QString getSeparator();
    /**
     * @brief Is the path specified a directory on the filesystem
     * @param path Path to examine
     */
    static EbsdLib_EXPORT bool isDirectory(const QString &path);

    /**
     * @brief Does the path designate a file on the file system
     * @param path Path to examine
     */
    static EbsdLib_EXPORT bool isFile(const QString &path);

    /**
     * @brief Returns true if the path is a relative path but does not determine
     * if the file actually exists or not
     * @@param path The path to check
     * @return True if the path is relative
     */
    static EbsdLib_EXPORT bool isRelativePath(const QString &path);

    /**
     * @brief Returns true if the path is an absolute path. On Unix this means the
     * first character is '/' and on windows the path starts with 'Drive:\' or '\\'
     * @@param path The path to check
     * @return True if the path is absolute
     */
    static EbsdLib_EXPORT bool isAbsolutePath(const QString &path);

    /**
     * @brief Returns the current path
     * @return The current working directory as reported by the operating system
     */
    static EbsdLib_EXPORT QString currentPath();

    /**
     * @brief Returns the path to the parent directory
     * @param path The path to return the parent path
     * @return The Parent path
     */
    static EbsdLib_EXPORT QString parentPath(const QString &path);


    /**
     * @brief Either calculates an absolute path or returns the same string if
     * it already indicates an absolute path. No Attempt is made to actually
     * determine if the file exists or not. The path will be free of any extra
     * './' or '..' in the path but symbolic links will possibly be in the path
     * @param path The path to check/convert
     * @return The absolute path.
     */
    static EbsdLib_EXPORT QString absolutePath(const QString &path);

    /**
     * @brief Does the path actually exist on the file system
     * @param path Path to examine
     */
    static EbsdLib_EXPORT bool exists(const QString &path);

    /**
     * @brief Cleans a file system path of extra './', '//' and '/../' elements
     * @param path Path to clean
     * @return A new string containing the cleaned path.
     */
    static EbsdLib_EXPORT QString cleanPath(const QString &path);

    /**
     * @brief Converts from native directory separators to unix separators
     * @param path The path to conver
     * @return Newly converted path
     */
    static EbsdLib_EXPORT QString fromNativeSeparators(const QString  &path);

    /**
     * @brief Converts a path to use native directory separators
     * @param path The path to convert
     * @return The newly converted path
     */
    static EbsdLib_EXPORT QString toNativeSeparators(const QString &path);

#if defined (WIN32)
    static EbsdLib_EXPORT bool isDirPath(const QString &path, bool *existed);
#endif


    // -----------------------------------------------------------------------------
    //  These are specific to MXAFileInfo
    // -----------------------------------------------------------------------------
    /**
     * @brief Get the size of the file in bytes
     */
    static EbsdLib_EXPORT uint64_t fileSize(const QString &path);

    /**
     * @brief Get any file extension on the filepath
     * @param path Path to examine
     */
    static EbsdLib_EXPORT QString extension(const QString &path);

    /**
     * @brief Return the filename WITH the extension
     * @param path Path to examine
     */
    static EbsdLib_EXPORT QString filename(const QString &path);

    /**
     * @brief Returns the filename without the extension
     * @param path Path to examine
     */
    static EbsdLib_EXPORT QString fileNameWithOutExtension(const QString &path);


protected:
    MXAFileInfo();

private:
    MXAFileInfo(const MXAFileInfo&);    // Copy Constructor Not Implemented
      void operator=(const MXAFileInfo&);  // Operator '=' Not Implemented
};

#endif /* MXAFILEINFO_H_ */
