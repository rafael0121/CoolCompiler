# start of generated code
	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	1
_bool_tag:
	.word	2
_string_tag:
	.word	0
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const14:
	.word	0
	.word	5
	.word	
	.word	int_const0
	.byte	0	
	.align	2
	.word	-1
str_const13:
	.word	0
	.word	6
	.word	
	.word	int_const1
	.ascii	"Child"
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	0
	.word	6
	.word	
	.word	int_const2
	.ascii	"Parent"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	0
	.word	7
	.word	
	.word	int_const3
	.ascii	"Grandparent"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	0
	.word	6
	.word	
	.word	int_const4
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	0
	.word	6
	.word	
	.word	int_const2
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	0
	.word	6
	.word	
	.word	int_const4
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	0
	.word	5
	.word	
	.word	int_const5
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	0
	.word	5
	.word	
	.word	int_const6
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	0
	.word	6
	.word	
	.word	int_const2
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	0
	.word	7
	.word	
	.word	int_const7
	.ascii	"_prim_slot"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	0
	.word	7
	.word	
	.word	int_const8
	.ascii	"SELF_TYPE"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	0
	.word	7
	.word	
	.word	int_const8
	.ascii	"_no_class"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	0
	.word	8
	.word	
	.word	int_const9
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	0
	.word	7
	.word	
	.word	int_const7
	.ascii	"example.cl"
	.byte	0	
	.align	2
	.word	-1
int_const9:
	.word	1
	.word	4
	.word	
	.word	13
	.word	-1
int_const8:
	.word	1
	.word	4
	.word	
	.word	9
	.word	-1
int_const7:
	.word	1
	.word	4
	.word	
	.word	10
	.word	-1
int_const6:
	.word	1
	.word	4
	.word	
	.word	2
	.word	-1
int_const5:
	.word	1
	.word	4
	.word	
	.word	3
	.word	-1
int_const4:
	.word	1
	.word	4
	.word	
	.word	4
	.word	-1
int_const3:
	.word	1
	.word	4
	.word	
	.word	11
	.word	-1
int_const2:
	.word	1
	.word	4
	.word	
	.word	6
	.word	-1
int_const1:
	.word	1
	.word	4
	.word	
	.word	5
	.word	-1
int_const0:
	.word	1
	.word	4
	.word	
	.word	0
	.word	-1
bool_const0:
	.word	2
	.word	4
	.word	
	.word	0
	.word	-1
bool_const1:
	.word	2
	.word	4
	.word	
	.word	1
	.word	-1
Child_protObj:
	.word	3
	.word	36
	.word	Child_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-1
Parent_protObj:
	.word	3
	.word	20
	.word	Parent_dispTab
	.word	0
	.word	0
	.word	-1
Grandparent_protObj:
	.word	3
	.word	12
	.word	Grandparent_dispTab
	.word	0
	.word	-1
Main_protObj:
	.word	3
	.word	4
	.word	Main_dispTab
	.word	-1
String_protObj:
	.word	3
	.word	20
	.word	String_dispTab
	.word	0
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	12
	.word	Bool_dispTab
	.word	0
	.word	-1
Int_protObj:
	.word	3
	.word	12
	.word	Int_dispTab
	.word	0
	.word	-1
IO_protObj:
	.word	3
	.word	4
	.word	IO_dispTab
	.word	-1
Object_protObj:
	.word	3
	.word	4
	.word	Object_dispTab
class_nameTab:
	.word	Child_protObj
	.word	Child_init
	.word	Parent_protObj
	.word	Parent_init
	.word	Grandparent_protObj
	.word	Grandparent_init
	.word	Main_protObj
	.word	Main_init
	.word	String_protObj
	.word	String_init
	.word	Bool_protObj
	.word	Bool_init
	.word	Int_protObj
	.word	Int_init
	.word	IO_protObj
	.word	IO_init
	.word	Object_protObj
	.word	Object_init
Child_dispTab
Parent_dispTab
Grandparent_dispTab
Main_dispTab
String_dispTab
Bool_dispTab
Int_dispTab
IO_dispTab
Object_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	1
_bool_tag:
	.word	2
_string_tag:
	.word	0
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const14:
	.word	0
	.word	5
	.word	
	.word	int_const0
	.byte	0	
	.align	2
	.word	-1
str_const13:
	.word	0
	.word	6
	.word	
	.word	int_const1
	.ascii	"Child"
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	0
	.word	6
	.word	
	.word	int_const2
	.ascii	"Parent"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	0
	.word	7
	.word	
	.word	int_const3
	.ascii	"Grandparent"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	0
	.word	6
	.word	
	.word	int_const4
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	0
	.word	6
	.word	
	.word	int_const2
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	0
	.word	6
	.word	
	.word	int_const4
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	0
	.word	5
	.word	
	.word	int_const5
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	0
	.word	5
	.word	
	.word	int_const6
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	0
	.word	6
	.word	
	.word	int_const2
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	0
	.word	7
	.word	
	.word	int_const7
	.ascii	"_prim_slot"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	0
	.word	7
	.word	
	.word	int_const8
	.ascii	"SELF_TYPE"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	0
	.word	7
	.word	
	.word	int_const8
	.ascii	"_no_class"
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	0
	.word	8
	.word	
	.word	int_const9
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	0
	.word	7
	.word	
	.word	int_const7
	.ascii	"example.cl"
	.byte	0	
	.align	2
	.word	-1
int_const9:
	.word	1
	.word	4
	.word	
	.word	13
	.word	-1
int_const8:
	.word	1
	.word	4
	.word	
	.word	9
	.word	-1
int_const7:
	.word	1
	.word	4
	.word	
	.word	10
	.word	-1
int_const6:
	.word	1
	.word	4
	.word	
	.word	2
	.word	-1
int_const5:
	.word	1
	.word	4
	.word	
	.word	3
	.word	-1
int_const4:
	.word	1
	.word	4
	.word	
	.word	4
	.word	-1
int_const3:
	.word	1
	.word	4
	.word	
	.word	11
	.word	-1
int_const2:
	.word	1
	.word	4
	.word	
	.word	6
	.word	-1
int_const1:
	.word	1
	.word	4
	.word	
	.word	5
	.word	-1
int_const0:
	.word	1
	.word	4
	.word	
	.word	0
	.word	-1
bool_const0:
	.word	2
	.word	4
	.word	
	.word	0
	.word	-1
bool_const1:
	.word	2
	.word	4
	.word	
	.word	1
	.word	-1
Child_protObj:
	.word	3
	.word	36
	.word	Child_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-1
Parent_protObj:
	.word	3
	.word	20
	.word	Parent_dispTab
	.word	0
	.word	0
	.word	-1
Grandparent_protObj:
	.word	3
	.word	12
	.word	Grandparent_dispTab
	.word	0
	.word	-1
Main_protObj:
	.word	3
	.word	4
	.word	Main_dispTab
	.word	-1
String_protObj:
	.word	3
	.word	20
	.word	String_dispTab
	.word	0
	.word	0
	.word	-1
Bool_protObj:
	.word	3
	.word	12
	.word	Bool_dispTab
	.word	0
	.word	-1
Int_protObj:
	.word	3
	.word	12
	.word	Int_dispTab
	.word	0
	.word	-1
IO_protObj:
	.word	3
	.word	4
	.word	IO_dispTab
	.word	-1
Object_protObj:
	.word	3
	.word	4
	.word	Object_dispTab
class_nameTab:
	.word	Child_protObj
	.word	Child_init
	.word	Parent_protObj
	.word	Parent_init
	.word	Grandparent_protObj
	.word	Grandparent_init
	.word	Main_protObj
	.word	Main_init
	.word	String_protObj
	.word	String_init
	.word	Bool_protObj
	.word	Bool_init
	.word	Int_protObj
	.word	Int_init
	.word	IO_protObj
	.word	IO_init
	.word	Object_protObj
	.word	Object_init
Child_dispTab
Parent_dispTab
Grandparent_dispTab
Main_dispTab
String_dispTab
Bool_dispTab
Int_dispTab
IO_dispTab
Object_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main

# end of generated code
