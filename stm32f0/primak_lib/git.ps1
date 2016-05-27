set-location ".\"

$FileName = "git_version_" + (get-item .).Name + ".h"
$defineName = "_GIT_VERSION_" + (get-item .).Name + "_H_"
$namespaceName = (get-item .).Name

if( Test-Path $FileName.ToString() ) { remove-item $FileName }
new-item -name $FileName -type file

"/*" | out-file $FileName -append
get-date | out-file $FileName -append
$str = &'C:\Program files (x86)\git\bin\git.exe' status
$str | out-file $FileName -append
"*/
" | out-file $FileName -append

("#ifndef " + $defineName) | out-file $FileName -append
("#define " + $defineName) | out-file $FileName -append
"
#include <ctime>" | out-file $FileName -append

("
namespace " + $namespaceName ) | out-file $FileName -append
"{" | out-file $FileName -append

$str = &'C:\Program files (x86)\git\bin\git.exe' rev-list HEAD --count
("  const unsigned int countFromHead = " + $str.ToString() + ";") | out-file $FileName -append
$str1 = &'C:\Program files (x86)\git\bin\git.exe' log --pretty=format:'%h' -n 1
("  const unsigned int hash = 0x" + $str1 + ";") | out-file $FileName -append
$str2 = &'C:\Program files (x86)\git\bin\git.exe' status --porcelain
if( $str2.length -eq 0 )
{
    ("  const unsigned int numUnrevisionedFiles = 0;") | out-file $FileName -append
}
elseif( $str2 -is "String" )
{
    ("  const unsigned int numUnrevisionedFiles = 1;") | out-file $FileName -append
}
elseif( $str2 -is "Object" )
{
    ("  const unsigned int numUnrevisionedFiles = " + $str2.length + ";") | out-file $FileName -append
}
else
{
	("  const unsigned int numUnrevisionedFiles = 0;") | out-file $FileName -append
}
"  const std::tm timeNow =
  {" | out-file $FileName -append
"    " + ([int](get-date -Format ss)).ToString() + "," | out-file $FileName -append
"    " + ([int](get-date -Format mm)).ToString() + "," | out-file $FileName -append
"    " + ([int](get-date -Format HH)).ToString() + "," | out-file $FileName -append
"    " + ([int](get-date -Format dd)).ToString() + "," | out-file $FileName -append
"    " + ([int](get-date -Format MM)).ToString() + "," | out-file $FileName -append
"    " + (get-date -Format yyyy).ToString() + " - 1900" | out-file $FileName -append
"  };" | out-file $FileName -append
"}
#endif" | out-file $FileName -append