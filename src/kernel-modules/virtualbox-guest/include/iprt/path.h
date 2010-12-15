 * Copyright (C) 2006-2007 Oracle Corporation
RT_C_DECLS_BEGIN
 * The preferred slash character.
 * The preferred slash character as a string, handy for concatenations
 * @remark  This is sufficient for the drive letter concept on PC.
/** @name Generic RTPath flags
 * @{ */
/** Last component: Work on the link. */
#define RTPATH_F_ON_LINK          RT_BIT_32(0)
/** Last component: Follow if link. */
#define RTPATH_F_FOLLOW_LINK      RT_BIT_32(1)
/** @} */


/** Validates a flags parameter containing RTPATH_F_*.
 * @remarks The parameters will be referneced multiple times. */
#define RTPATH_F_IS_VALID(fFlags, fIgnore) \
    (    ((fFlags) & ~(uint32_t)(fIgnore)) == RTPATH_F_ON_LINK \
      || ((fFlags) & ~(uint32_t)(fIgnore)) == RTPATH_F_FOLLOW_LINK )


 * Symbolic links will all be attempted resolved and broken links means false.
/**
 * Checks if the path exists.
 *
 * @returns true if it exists and false if it doesn't.
 * @param   pszPath     The path to check.
 * @param   fFlags      RTPATH_F_ON_LINK or RTPATH_F_FOLLOW_LINK.
 */
RTDECL(bool) RTPathExistsEx(const char *pszPath, uint32_t fFlags);

/**
 * Sets the current working directory of the process.
 *
 * @returns IPRT status code.
 * @param   pszPath         The path to the new working directory.
 */
RTDECL(int)  RTPathSetCurrent(const char *pszPath);

/**
 * Gets the current working directory of the process.
 *
 * @returns IPRT status code.
 * @param   pszPath         Where to store the path.
 * @param   cchPath         The size of the buffer pszPath points to.
 */
RTDECL(int)  RTPathGetCurrent(char *pszPath, size_t cchPath);

RTDECL(int) RTPathReal(const char *pszPath, char *pszRealPath, size_t cchRealPath);
 * Get the absolute path (starts from root, no . or .. components), doesn't have
 * to exist. Note that this method is designed to never perform actual file
 * system access, therefore symlinks are not resolved.
RTDECL(int) RTPathAbs(const char *pszPath, char *pszAbsPath, size_t cchAbsPath);
RTDECL(int) RTPathAbsEx(const char *pszBase, const char *pszPath, char *pszAbsPath, size_t cchAbsPath);
 * Strips the filename from a path. Truncates the given string in-place by overwriting the
 * last path separator character with a null byte in a platform-neutral way.
 * @param   pszPath     Path from which filename should be extracted, will be truncated.
 *                      If the string contains no path separator, it will be changed to a "." string.
 * Won't strip root slashes.
 *
 * @returns The new length of pszPath.
RTDECL(size_t) RTPathStripTrailingSlash(char *pszPath);

/**
 * Changes all the slahes in the specified path to DOS style.
 *
 * Unless @a fForce is set, nothing will be done when on a UNIX flavored system
 * since paths wont work with DOS style slashes there.
 *
 * @returns @a pszPath.
 * @param   pszPath             The path to modify.
 * @param   fForce              Whether to force the conversion on non-DOS OSes.
 */
RTDECL(char *) RTPathChangeToDosSlashes(char *pszPath, bool fForce);

/**
 * Changes all the slahes in the specified path to unix style.
 *
 * Unless @a fForce is set, nothing will be done when on a UNIX flavored system
 * since paths wont work with DOS style slashes there.
 *
 * @returns @a pszPath.
 * @param   pszPath             The path to modify.
 * @param   fForce              Whether to force the conversion on non-DOS OSes.
 */
RTDECL(char *) RTPathChangeToUnixSlashes(char *pszPath, bool fForce);

/**
 * Parses a path.
 *
 * It figures the length of the directory component, the offset of
 * the file name and the location of the suffix dot.
 *
 * @returns The path length.
 *
 * @param   pszPath     Path to find filename in.
 * @param   pcchDir     Where to put the length of the directory component. If
 *                      no directory, this will be 0. Optional.
 * @param   poffName    Where to store the filename offset.
 *                      If empty string or if it's ending with a slash this
 *                      will be set to -1. Optional.
 * @param   poffSuff    Where to store the suffix offset (the last dot).
 *                      If empty string or if it's ending with a slash this
 *                      will be set to -1. Optional.
 */
RTDECL(size_t) RTPathParse(const char *pszPath, size_t *pcchDir, ssize_t *poffName, ssize_t *poffSuff);
/**
 * Counts the components in the specified path.
 *
 * An empty string has zero components.  A lone root slash is considered have
 * one.  The paths "/init" and "/bin/" are considered having two components.  An
 * UNC share specifier like "\\myserver\share" will be considered as one single
 * component.
 *
 * @returns The number of path components.
 * @param   pszPath     The path to parse.
 */
RTDECL(size_t) RTPathCountComponents(const char *pszPath);

/**
 * Copies the specified number of path components from @a pszSrc and into @a
 * pszDst.
 *
 * @returns VINF_SUCCESS or VERR_BUFFER_OVERFLOW.  In the latter case the buffer
 *          is not touched.
 *
 * @param   pszDst      The destination buffer.
 * @param   cbDst       The size of the destination buffer.
 * @param   pszSrc      The source path.
 * @param   cComponents The number of components to copy from @a pszSrc.
 */
RTDECL(int) RTPathCopyComponents(char *pszDst, size_t cbDst, const char *pszSrc, size_t cComponents);

 * <li>On DOS-like platforms, both separator chars (|\| and |/|) are considered
 * @returns @< 0 if the first path less than the second path.
 * @returns 0 if the first path identical to the second path.
 * @returns @> 0 if the first path greater than the second path.
 * @remarks File system details are currently ignored. This means that you won't
 *          get case-insentive compares on unix systems when a path goes into a
 *          case-insensitive filesystem like FAT, HPFS, HFS, NTFS, JFS, or
 *          similar. For NT, OS/2 and similar you'll won't get case-sensitve
 *          compares on a case-sensitive file system.
 * This means that either the path and the parent path matches completely, or
 * that the path is to some file or directory residing in the tree given by the
 * parent directory.
 * @returns |true| when \a pszPath starts with \a pszParentPath (or when they
 *          are identical), or |false| otherwise.
 *
 * @remarks This API doesn't currently handle root directory compares in a
 *          manner consistant with the other APIs. RTPathStartsWith(pszSomePath,
 *          "/") will not work if pszSomePath isn't "/".
/**
 * Appends one partial path to another.
 *
 * The main purpose of this function is to deal correctly with the slashes when
 * concatenating the two partial paths.
 *
 * @retval  VINF_SUCCESS on success.
 * @retval  VERR_BUFFER_OVERFLOW if the result is too big to fit within
 *          cbPathDst bytes. No changes has been made.
 * @retval  VERR_INVALID_PARAMETER if the string pointed to by pszPath is longer
 *          than cbPathDst-1 bytes (failed to find terminator). Asserted.
 *
 * @param   pszPath         The path to append pszAppend to. This serves as both
 *                          input and output. This can be empty, in which case
 *                          pszAppend is just copied over.
 * @param   cbPathDst       The size of the buffer pszPath points to, terminator
 *                          included. This should NOT be strlen(pszPath).
 * @param   pszAppend       The partial path to append to pszPath. This can be
 *                          NULL, in which case nothing is done.
 *
 * @remarks On OS/2, Window and similar systems, concatenating a drive letter
 *          specifier with a slash prefixed path will result in an absolute
 *          path. Meaning, RTPathAppend(strcpy(szBuf, "C:"), sizeof(szBuf),
 *          "/bar") will result in "C:/bar". (This follows directly from the
 *          behavior when pszPath is empty.)
 *
 *          On the other hand, when joining a drive letter specifier with a
 *          partial path that does not start with a slash, the result is not an
 *          absolute path. Meaning, RTPathAppend(strcpy(szBuf, "C:"),
 *          sizeof(szBuf), "bar") will result in "C:bar".
 */
RTDECL(int) RTPathAppend(char *pszPath, size_t cbPathDst, const char *pszAppend);

/**
 * Like RTPathAppend, but with the base path as a separate argument instead of
 * in the path buffer.
 *
 * @retval  VINF_SUCCESS on success.
 * @retval  VERR_BUFFER_OVERFLOW if the result is too big to fit within
 *          cbPathDst bytes.
 * @retval  VERR_INVALID_PARAMETER if the string pointed to by pszPath is longer
 *          than cbPathDst-1 bytes (failed to find terminator). Asserted.
 *
 * @param   pszPathDst      Where to store the resulting path.
 * @param   cbPathDst       The size of the buffer pszPathDst points to,
 *                          terminator included.
 * @param   pszPathSrc      The base path to copy into @a pszPathDst before
 *                          appending @a pszAppend.
 * @param   pszAppend       The partial path to append to pszPathSrc. This can
 *                          be NULL, in which case nothing is done.
 *
 */
RTDECL(int) RTPathJoin(char *pszPathDst, size_t cbPathDst, const char *pszPathSrc,
                       const char *pszAppend);

/**
 * Callback for RTPathTraverseList that's called for each element.
 *
 * @returns IPRT style status code. Return VINF_TRY_AGAIN to continue, any other
 *          value will abort the traversing and be returned to the caller.
 *
 * @param   pchPath         Pointer to the start of the current path. This is
 *                          not null terminated.
 * @param   cchPath         The length of the path.
 * @param   pvUser1         The first user parameter.
 * @param   pvUser2         The second user parameter.
 */
typedef DECLCALLBACK(int) FNRTPATHTRAVERSER(char const *pchPath, size_t cchPath, void *pvUser1, void *pvUser2);
/** Pointer to a FNRTPATHTRAVERSER. */
typedef FNRTPATHTRAVERSER *PFNRTPATHTRAVERSER;

/**
 * Traverses a string that can contain multiple paths separated by a special
 * character.
 *
 * @returns IPRT style status code from the callback or VERR_END_OF_STRING if
 *          the callback returned VINF_TRY_AGAIN for all paths in the string.
 *
 * @param   pszPathList     The string to traverse.
 * @param   chSep           The separator character.  Using the null terminator
 *                          is fine, but the result will simply be that there
 *                          will only be one callback for the entire string
 *                          (save any leading white space).
 * @param   pfnCallback     The callback.
 * @param   pvUser1         First user argument for the callback.
 * @param   pvUser2         Second user argument for the callback.
 */
RTDECL(int) RTPathTraverseList(const char *pszPathList, char chSep, PFNRTPATHTRAVERSER pfnCallback, void *pvUser1, void *pvUser2);

 * Gets the path to the directory containing the executable.
RTDECL(int) RTPathExecDir(char *pszPath, size_t cchPath);
RTDECL(int) RTPathUserHome(char *pszPath, size_t cchPath);
 * Gets the directory of shared libraries.
 *
 * This is not the same as RTPathAppPrivateArch() as Linux depends all shared
 * libraries in a common global directory where ld.so can found them.
 * Old path: same as RTPathExecDir()
RTDECL(int) RTPathSharedLibs(char *pszPath, size_t cchPath);
 * Old path: same as RTPathExecDir()
RTDECL(int) RTPathAppPrivateNoArch(char *pszPath, size_t cchPath);
 * Old path: same as RTPathExecDir()
RTDECL(int) RTPathAppPrivateArch(char *pszPath, size_t cchPath);
 * Old path: same as RTPathExecDir()
RTDECL(int) RTPathAppDocs(char *pszPath, size_t cchPath);

/**
 * Gets the temporary directory path.
 *
 * @returns iprt status code.
 * @param   pszPath     Buffer where to store the path.
 * @param   cchPath     Buffer size in bytes.
 */
RTDECL(int) RTPathTemp(char *pszPath, size_t cchPath);
 * This API will resolve NOT symbolic links in the last component (just like
 * unix lstat()).
 * @returns IPRT status code.
 * @retval  VINF_SUCCESS if the object exists, information returned.
 * @retval  VERR_PATH_NOT_FOUND if any but the last component in the specified
 * @retval  VERR_FILE_NOT_FOUND if the object does not exist (but path to the
/**
 * Query information about a file system object.
 *
 * @returns IPRT status code.
 * @retval  VINF_SUCCESS if the object exists, information returned.
 * @retval  VERR_PATH_NOT_FOUND if any but the last component in the specified
 *          path was not found or was not a directory.
 * @retval  VERR_FILE_NOT_FOUND if the object does not exist (but path to the
 *          parent directory exists).
 *
 * @param   pszPath     Path to the file system object.
 * @param   pObjInfo    Object information structure to be filled on successful return.
 * @param   enmAdditionalAttribs
 *                      Which set of additional attributes to request.
 *                      Use RTFSOBJATTRADD_NOTHING if this doesn't matter.
 * @param   fFlags      RTPATH_F_ON_LINK or RTPATH_F_FOLLOW_LINK.
 */
RTR3DECL(int) RTPathQueryInfoEx(const char *pszPath, PRTFSOBJINFO pObjInfo, RTFSOBJATTRADD enmAdditionalAttribs, uint32_t fFlags);

 * @param   pModificationTime   Pointer to the new modification time.
/**
 * Changes one or more of the timestamps associated of file system object.
 *
 * @returns iprt status code.
 * @param   pszPath             Path to the file system object.
 * @param   pAccessTime         Pointer to the new access time.
 * @param   pModificationTime   Pointer to the new modification time.
 * @param   pChangeTime         Pointer to the new change time. NULL if not to be changed.
 * @param   pBirthTime          Pointer to the new time of birth. NULL if not to be changed.
 * @param   fFlags              RTPATH_F_ON_LINK or RTPATH_F_FOLLOW_LINK.
 *
 * @remark  The file system might not implement all these time attributes,
 *          the API will ignore the ones which aren't supported.
 *
 * @remark  The file system might not implement the time resolution
 *          employed by this interface, the time will be chopped to fit.
 *
 * @remark  The file system may update the change time even if it's
 *          not specified.
 *
 * @remark  POSIX can only set Access & Modification and will always set both.
 */
RTR3DECL(int) RTPathSetTimesEx(const char *pszPath, PCRTTIMESPEC pAccessTime, PCRTTIMESPEC pModificationTime,
                               PCRTTIMESPEC pChangeTime, PCRTTIMESPEC pBirthTime, uint32_t fFlags);

 * @param   pModificationTime   Where to store the modification time. NULL is ok.
 * @remark  This is wrapper around RTPathQueryInfo() and exists to complement
 *          RTPathSetTimes().  If the last component is a symbolic link, it will
 *          not be resolved.
/**
 * Changes the owner and/or group of a file system object.
 *
 * @returns iprt status code.
 * @param   pszPath     Path to the file system object.
 * @param   uid         The new file owner user id. Use -1 (or ~0) to leave this unchanged.
 * @param   gid         The new group id. Use -1 (or ~0) to leave this unchanged.
 * @param   fFlags      RTPATH_F_ON_LINK or RTPATH_F_FOLLOW_LINK.
 */
RTR3DECL(int) RTPathSetOwnerEx(const char *pszPath, uint32_t uid, uint32_t gid, uint32_t fFlags);

 * @remark  This is wrapper around RTPathQueryInfo() and exists to complement
 *          RTPathGetOwner().  If the last component is a symbolic link, it will
 *          not be resolved.
 * This will rename symbolic links.  If RTPATHRENAME_FLAGS_REPLACE is used and
 * pszDst is a symbolic link, it will be replaced and not its target.
 *
RT_C_DECLS_END