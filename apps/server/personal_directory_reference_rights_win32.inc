/* A metadata reference does not request data/traversal access. The original
 * parent retaining ADD_SUBDIRECTORY remains the sole traversal/create source.
 * Never broaden sharing to accommodate an unnecessary second data open. */
static ACCESS_MASK scqi_directory_rights(int metadata)
{return FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE|(metadata?0U:FILE_TRAVERSE);}
