/*
* Paula.cpp
* ---------
* Purpose: Emulating the Amiga's sound chip, Paula, by implementing resampling using band-limited steps (BLEPs)
* Notes  : (currently none)
* Authors: OpenMPT Devs
*          Antti S. Lankila
* The OpenMPT source code is released under the BSD license. Read LICENSE for more details.
*/

#include "stdafx.h"
#include "Paula.h"

OPENMPT_NAMESPACE_BEGIN

namespace Paula
{

// Tables are: A500 (filter off), A500 (filter on)
static constexpr int32 WinSincIntegral[2][2048] =
{
	{
	131072,131072,131072,131072,131072,131072,131072,131072,131072,131072,131072,
	131072,131072,131072,131072,131072,131072,131072,131072,131072,131072,131071,131071,
	131071,131071,131071,131071,131071,131071,131071,131071,131071,131070,131070,131070,
	131070,131070,131069,131069,131069,131068,131068,131068,131067,131067,131066,131066,
	131065,131065,131064,131063,131063,131062,131061,131060,131059,131058,131056,131055,
	131054,131052,131050,131049,131047,131045,131043,131040,131038,131035,131033,131030,
	131026,131023,131020,131016,131012,131008,131003,130998,130993,130988,130982,130976,
	130970,130963,130956,130949,130941,130932,130924,130914,130905,130895,130884,130872,
	130861,130848,130835,130821,130807,130792,130776,130759,130742,130724,130705,130685,
	130664,130642,130620,130596,130571,130545,130518,130490,130461,130430,130398,130365,
	130331,130295,130257,130219,130178,130136,130093,130047,130000,129951,129901,129848,
	129794,129737,129679,129618,129555,129490,129423,129353,129281,129207,129130,129050,
	128968,128883,128795,128704,128611,128514,128415,128312,128206,128097,127985,127869,
	127750,127627,127501,127371,127237,127100,126959,126813,126664,126510,126353,126191,
	126025,125854,125679,125499,125315,125126,124933,124734,124531,124323,124110,123891,
	123668,123439,123205,122965,122720,122470,122214,121952,121685,121412,121133,120849,
	120558,120261,119959,119650,119335,119014,118687,118354,118014,117668,117315,116956,
	116591,116219,115840,115455,115063,114665,114260,113849,113430,113005,112574,112135,
	111690,111239,110780,110315,109843,109364,108879,108387,107888,107383,106871,106352,
	105827,105295,104757,104212,103661,103104,102540,101970,101394,100812,100223,99629,
	99028,98422,97810,97192,96568,95939,95305,94665,94020,93370,92714,92054,91389,90719,
	90045,89366,88682,87995,87303,86607,85908,85205,84498,83788,83075,82358,81639,80916,
	80191,79464,78734,78002,77268,76533,75795,75056,74316,73575,72833,72090,71346,70602,
	69858,69114,68370,67626,66883,66140,65399,64658,63919,63181,62445,61711,60979,60249,
	59521,58796,58074,57355,56639,55926,55217,54512,53810,53113,52419,51731,51046,50367,
	49693,49023,48359,47701,47048,46400,45759,45124,44495,43872,43256,42646,42043,41447,
	40858,40276,39702,39134,38575,38023,37478,36941,36413,35892,35379,34874,34378,33890,
	33410,32938,32475,32020,31574,31137,30708,30288,29876,29473,29079,28693,28317,27948,
	27589,27238,26896,26562,26238,25921,25613,25314,25023,24740,24466,24200,23942,23692,
	23451,23217,22991,22773,22562,22359,22164,21975,21794,21621,21454,21294,21140,20994,
	20853,20719,20592,20470,20354,20244,20139,20040,19946,19857,19774,19694,19620,19550,
	19484,19422,19364,19310,19260,19213,19169,19128,19090,19054,19022,18991,18963,18936,
	18912,18889,18867,18847,18828,18810,18792,18776,18759,18743,18727,18711,18695,18679,
	18662,18644,18626,18607,18587,18565,18542,18518,18492,18465,18436,18404,18371,18336,
	18298,18259,18216,18172,18124,18074,18022,17966,17908,17847,17783,17716,17646,17572,
	17496,17416,17334,17248,17159,17066,16971,16872,16770,16664,16556,16444,16329,16211,
	16090,15966,15839,15709,15576,15440,15301,15159,15015,14868,14718,14566,14412,14255,
	14096,13935,13771,13606,13439,13270,13099,12927,12753,12578,12401,12224,12045,11866,
	11685,11504,11322,11140,10958,10775,10592,10409,10226,10044,9862,9680,9499,9319,9139,
	8961,8783,8607,8432,8258,8086,7915,7747,7580,7415,7252,7091,6932,6776,6622,6471,
	6322,6176,6032,5892,5754,5619,5488,5359,5234,5111,4992,4877,4764,4655,4550,4448,
	4349,4254,4163,4075,3990,3910,3832,3759,3689,3622,3560,3500,3445,3393,3344,3299,
	3257,3219,3184,3153,3124,3099,3078,3059,3044,3031,3022,3015,3011,3010,3012,3016,
	3023,3033,3044,3058,3075,3093,3113,3136,3160,3186,3213,3242,3273,3305,3338,3372,
	3408,3444,3481,3520,3558,3597,3637,3677,3718,3758,3799,3839,3880,3920,3960,4000,
	4039,4077,4115,4152,4188,4224,4258,4291,4323,4354,4384,4412,4439,4464,4488,4510,
	4530,4549,4566,4581,4594,4606,4615,4623,4628,4631,4633,4632,4629,4624,4617,4608,
	4597,4583,4568,4550,4530,4508,4484,4458,4429,4399,4366,4332,4296,4257,4217,4175,
	4130,4085,4037,3988,3937,3884,3830,3774,3717,3658,3598,3537,3475,3411,3347,3281,
	3215,3147,3079,3010,2940,2870,2799,2728,2657,2585,2513,2440,2368,2296,2224,2151,
	2080,2008,1937,1866,1796,1726,1657,1589,1521,1454,1389,1324,1260,1197,1135,1075,
	1016,958,901,846,792,740,689,640,592,546,502,459,419,379,342,307,273,241,211,183,
	156,132,109,88,69,52,37,24,12,2,-5,-11,-16,-18,-19,-18,-16,-11,-6,2,11,21,33,47,61,
	77,95,113,133,154,176,200,224,249,275,302,329,358,387,416,447,477,508,540,572,604,
	636,669,702,734,767,800,832,864,896,928,960,991,1021,1051,1081,1110,1138,1166,1193,
	1219,1245,1270,1293,1316,1338,1359,1379,1398,1416,1433,1448,1463,1476,1488,1499,
	1509,1518,1525,1531,1536,1540,1542,1543,1543,1542,1539,1536,1530,1524,1517,1508,
	1498,1487,1475,1462,1447,1432,1415,1397,1379,1359,1338,1317,1294,1271,1247,1222,
	1196,1170,1143,1115,1086,1057,1028,998,967,936,905,874,842,809,777,744,712,679,646,
	613,581,548,515,483,450,418,387,355,324,293,263,233,204,175,147,119,92,66,40,15,
	-10,-33,-56,-78,-99,-120,-139,-158,-176,-193,-209,-224,-238,-252,-264,-275,-286,
	-295,-304,-311,-318,-324,-329,-332,-335,-337,-338,-338,-338,-336,-333,-330,-326,
	-321,-315,-308,-301,-293,-284,-274,-264,-253,-242,-229,-217,-203,-190,-175,-161,
	-145,-130,-114,-98,-81,-64,-47,-29,-11,6,24,42,61,79,97,115,134,152,170,188,206,223,
	241,258,275,291,308,324,340,355,370,384,399,412,425,438,450,462,473,484,494,503,
	512,521,529,536,542,548,553,558,562,566,568,570,572,573,573,573,572,570,568,565,
	562,558,553,548,543,537,530,523,515,507,498,489,479,469,459,448,437,426,414,402,
	389,377,364,351,337,324,310,296,282,268,254,239,225,211,196,182,168,153,139,125,
	111,97,83,70,56,43,30,17,5,-7,-19,-31,-42,-53,-64,-75,-85,-94,-104,-113,-121,-129,
	-137,-144,-151,-158,-164,-170,-175,-180,-184,-188,-192,-195,-198,-200,-202,-203,
	-204,-205,-205,-204,-204,-203,-201,-199,-197,-195,-192,-188,-185,-181,-176,-172,
	-167,-162,-156,-151,-145,-139,-132,-126,-119,-112,-104,-97,-90,-82,-74,-66,-59,-51,
	-42,-34,-26,-18,-10,-2,7,15,23,31,39,47,54,62,70,77,85,92,99,106,112,119,125,131,
	137,143,148,154,159,163,168,172,176,180,183,187,190,192,195,197,199,201,202,203,
	204,205,205,205,205,204,203,202,201,200,198,196,194,192,189,186,183,180,177,173,
	169,165,161,157,153,148,143,139,134,129,124,119,113,108,103,97,92,86,81,75,70,64,
	59,53,48,42,37,32,26,21,16,11,6,1,-4,-9,-13,-18,-22,-26,-30,-34,-38,-42,-46,-49,
	-52,-55,-58,-61,-64,-66,-69,-71,-73,-74,-76,-78,-79,-80,-81,-82,-82,-83,-83,-83,
	-83,-83,-83,-82,-82,-81,-80,-79,-78,-77,-75,-74,-72,-70,-68,-66,-64,-62,-60,-57,
	-55,-52,-50,-47,-44,-42,-39,-36,-33,-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,6,8,
	11,14,17,20,22,25,27,30,32,35,37,39,41,43,45,47,49,50,52,54,55,56,58,59,60,61,61,
	62,63,63,64,64,65,65,65,65,65,64,64,64,63,63,62,61,61,60,59,58,57,56,55,53,52,51,
	49,48,46,45,43,41,40,38,36,35,33,31,29,28,26,24,22,20,19,17,15,13,11,10,8,6,5,3,1,
	0,-2,-3,-5,-6,-8,-9,-10,-12,-13,-14,-15,-16,-17,-18,-19,-20,-21,-21,-22,-23,-23,
	-24,-24,-25,-25,-26,-26,-26,-26,-26,-26,-26,-26,-26,-26,-26,-26,-25,-25,-25,-24,
	-24,-23,-23,-22,-21,-21,-20,-19,-19,-18,-17,-16,-16,-15,-14,-13,-12,-11,-10,-10,-9,
	-8,-7,-6,-5,-4,-3,-2,-1,0,1,1,2,3,4,5,6,6,7,8,9,9,10,11,11,12,12,13,14,14,15,15,
	15,16,16,16,17,17,17,17,18,18,18,18,18,18,18,18,18,18,18,18,18,17,17,17,17,16,16,
	16,16,15,15,14,14,14,13,13,12,12,11,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,
	1,0,0,-1,-1,-1,-2,-2,-3,-3,-3,-4,-4,-4,-4,-5,-5,-5,-5,-6,-6,-6,-6,-6,-6,-6,-7,-7,
	-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-7,-6,-6,-6,-6,-6,-6,-6,-6,-5,-5,-5,-5,-5,-4,-4,-4,
	-4,-4,-3,-3,-3,-3,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,3,3,
	3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,
	3,3,3,3,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	},
	{
	131072,131072,131072,131072,131072,131072,131072,131072,131072,131072,131072,
	131072,131071,131071,131071,131071,131071,131071,131071,131071,131071,131071,131071,
	131071,131071,131071,131071,131071,131071,131071,131071,131071,131071,131071,131071,
	131070,131070,131070,131070,131070,131070,131070,131070,131070,131070,131070,131070,
	131070,131070,131070,131070,131070,131070,131070,131070,131070,131070,131069,131069,
	131069,131069,131069,131069,131069,131069,131069,131069,131069,131069,131069,131069,
	131069,131068,131068,131068,131068,131068,131068,131068,131068,131068,131068,131067,
	131067,131067,131067,131067,131067,131067,131066,131066,131066,131066,131066,131065,
	131065,131065,131065,131064,131064,131064,131064,131063,131063,131062,131062,131062,
	131061,131061,131060,131060,131059,131059,131058,131058,131057,131056,131056,131055,
	131054,131053,131052,131052,131051,131050,131049,131048,131046,131045,131044,131043,
	131041,131040,131038,131037,131035,131034,131032,131030,131028,131026,131024,131022,
	131020,131017,131015,131012,131010,131007,131004,131001,130998,130994,130991,130987,
	130984,130980,130976,130972,130968,130963,130959,130954,130949,130944,130938,130933,
	130927,130921,130915,130909,130902,130895,130888,130881,130873,130865,130857,130849,
	130840,130831,130822,130813,130803,130793,130782,130771,130760,130749,130737,130725,
	130712,130699,130685,130672,130657,130643,130628,130612,130596,130580,130563,130545,
	130528,130509,130490,130471,130451,130430,130409,130388,130365,130343,130319,130295,
	130270,130245,130219,130193,130165,130137,130109,130079,130049,130018,129987,129954,
	129921,129887,129853,129817,129781,129744,129706,129667,129627,129587,129545,129503,
	129459,129415,129370,129324,129276,129228,129179,129129,129078,129026,128972,128918,
	128862,128806,128748,128690,128630,128569,128507,128443,128379,128313,128246,128178,
	128109,128038,127966,127893,127819,127743,127667,127588,127509,127428,127346,127262,
	127177,127091,127004,126915,126824,126732,126639,126545,126449,126351,126252,126152,
	126050,125947,125842,125736,125628,125519,125408,125296,125182,125067,124950,124832,
	124712,124591,124468,124344,124218,124090,123961,123830,123698,123564,123429,123292,
	123154,123014,122872,122729,122585,122438,122291,122141,121990,121838,121684,121528,
	121371,121212,121052,120891,120727,120562,120396,120228,120059,119888,119715,119541,
	119366,119189,119011,118831,118649,118467,118282,118097,117909,117721,117531,117339,
	117146,116952,116757,116560,116361,116161,115960,115758,115554,115349,115143,114935,
	114726,114516,114305,114092,113878,113663,113447,113229,113011,112791,112570,112348,
	112124,111900,111675,111448,111220,110992,110762,110531,110300,110067,109833,109598,
	109363,109126,108889,108650,108411,108171,107930,107688,107445,107201,106957,106712,
	106466,106219,105972,105723,105474,105225,104974,104723,104471,104219,103966,103712,
	103458,103203,102948,102692,102435,102178,101920,101662,101403,101144,100885,100624,
	100364,100103,99841,99579,99317,99054,98791,98527,98264,97999,97735,97470,97204,96939,
	96673,96407,96140,95873,95606,95339,95071,94804,94536,94267,93999,93730,93461,93192,
	92923,92653,92383,92114,91844,91573,91303,91033,90762,90491,90220,89949,89678,89407,
	89136,88865,88593,88321,88050,87778,87506,87234,86962,86690,86418,86146,85874,85602,
	85330,85057,84785,84513,84240,83968,83696,83423,83151,82878,82606,82333,82061,81789,
	81516,81244,80971,80699,80427,80154,79882,79610,79337,79065,78793,78521,78249,77977,
	77705,77433,77161,76889,76617,76345,76074,75802,75531,75259,74988,74717,74446,74175,
	73904,73633,73362,73092,72821,72551,72280,72010,71740,71470,71201,70931,70661,70392,
	70123,69854,69585,69317,69048,68780,68512,68244,67976,67709,67441,67174,66907,66641,
	66374,66108,65842,65576,65311,65046,64781,64516,64252,63988,63724,63460,63197,62934,
	62672,62409,62147,61886,61624,61363,61103,60843,60583,60323,60064,59805,59547,59289,
	59031,58774,58517,58261,58005,57749,57494,57239,56985,56731,56478,56225,55973,55721,
	55470,55219,54968,54718,54469,54220,53971,53724,53476,53229,52983,52737,52492,52247,
	52003,51759,51516,51274,51032,50791,50550,50310,50070,49831,49593,49355,49118,48881,
	48645,48410,48175,47941,47707,47474,47242,47010,46779,46549,46319,46090,45861,45633,
	45406,45179,44953,44728,44503,44279,44056,43833,43611,43389,43168,42948,42728,42510,
	42291,42074,41857,41640,41425,41210,40995,40782,40568,40356,40144,39933,39723,39513,
	39304,39095,38887,38680,38473,38267,38062,37857,37653,37449,37247,37044,36843,36642,
	36442,36242,36043,35845,35647,35450,35253,35057,34862,34667,34473,34280,34087,33894,
	33703,33512,33321,33132,32942,32754,32566,32378,32192,32006,31820,31635,31451,31267,
	31084,30901,30719,30538,30357,30177,29997,29818,29640,29462,29285,29108,28932,28756,
	28581,28407,28233,28060,27888,27716,27545,27374,27204,27034,26865,26697,26529,26362,
	26195,26029,25863,25699,25534,25371,25207,25045,24883,24722,24561,24401,24241,24082,
	23924,23766,23609,23453,23297,23142,22987,22833,22679,22526,22374,22222,22071,21921,
	21771,21622,21473,21325,21177,21031,20884,20739,20594,20449,20306,20162,20020,19878,
	19737,19596,19456,19316,19178,19039,18902,18765,18628,18493,18358,18223,18089,17956,
	17823,17691,17560,17429,17299,17169,17040,16912,16784,16657,16531,16405,16280,16155,
	16031,15908,15785,15663,15541,15420,15300,15180,15061,14942,14824,14707,14590,14474,
	14358,14243,14129,14015,13901,13789,13677,13565,13454,13344,13234,13125,13016,12908,
	12801,12694,12588,12482,12377,12272,12168,12064,11961,11859,11757,11655,11554,11454,
	11354,11255,11156,11058,10960,10863,10766,10670,10575,10480,10385,10291,10197,10104,
	10012,9920,9828,9737,9646,9556,9466,9377,9289,9200,9113,9025,8939,8852,8766,8681,
	8596,8512,8428,8344,8261,8178,8096,8014,7933,7852,7772,7692,7612,7533,7455,7376,
	7299,7221,7144,7068,6992,6916,6841,6766,6692,6618,6544,6471,6398,6326,6254,6182,
	6111,6041,5970,5901,5831,5762,5693,5625,5557,5490,5423,5356,5290,5224,5159,5093,
	5029,4965,4901,4837,4774,4711,4649,4587,4525,4464,4404,4343,4283,4224,4164,4105,
	4047,3989,3931,3874,3817,3760,3704,3648,3593,3538,3483,3429,3375,3321,3268,3215,
	3163,3111,3059,3008,2957,2906,2856,2806,2756,2707,2658,2610,2562,2514,2466,2419,
	2373,2326,2280,2235,2189,2144,2100,2056,2012,1968,1925,1882,1839,1797,1755,1714,
	1672,1631,1591,1551,1511,1471,1432,1393,1354,1316,1278,1240,1203,1165,1129,1092,
	1056,1020,984,949,914,879,845,811,777,743,710,677,644,612,580,548,516,485,454,423,
	393,362,332,303,273,244,215,186,158,129,101,74,46,19,-8,-35,-61,-88,-114,-140,-165,
	-191,-216,-241,-265,-290,-314,-338,-362,-386,-409,-432,-455,-478,-501,-523,-545,
	-567,-589,-610,-632,-653,-674,-694,-715,-735,-756,-775,-795,-815,-834,-853,-872,
	-891,-910,-928,-947,-965,-983,-1000,-1018,-1035,-1052,-1069,-1086,-1103,-1119,-1136,
	-1152,-1168,-1184,-1199,-1215,-1230,-1245,-1260,-1275,-1289,-1304,-1318,-1332,-1346,
	-1360,-1373,-1387,-1400,-1413,-1426,-1439,-1452,-1464,-1477,-1489,-1501,-1513,-1524,
	-1536,-1547,-1558,-1570,-1581,-1591,-1602,-1612,-1623,-1633,-1643,-1653,-1663,-1672,
	-1682,-1691,-1700,-1709,-1718,-1727,-1735,-1744,-1752,-1760,-1768,-1776,-1784,-1792,
	-1799,-1807,-1814,-1821,-1828,-1835,-1842,-1848,-1855,-1861,-1867,-1873,-1879,-1885,
	-1891,-1896,-1902,-1907,-1913,-1918,-1923,-1928,-1932,-1937,-1942,-1946,-1950,-1955,
	-1959,-1963,-1967,-1971,-1974,-1978,-1981,-1985,-1988,-1991,-1994,-1997,-2000,-2003,
	-2006,-2009,-2011,-2013,-2016,-2018,-2020,-2022,-2024,-2026,-2028,-2030,-2032,-2033,
	-2035,-2036,-2037,-2039,-2040,-2041,-2042,-2043,-2044,-2045,-2045,-2046,-2047,-2047,
	-2048,-2048,-2048,-2049,-2049,-2049,-2049,-2049,-2049,-2049,-2049,-2048,-2048,-2048,
	-2047,-2047,-2046,-2046,-2045,-2044,-2043,-2043,-2042,-2041,-2040,-2039,-2038,-2036,
	-2035,-2034,-2033,-2031,-2030,-2028,-2027,-2025,-2024,-2022,-2020,-2019,-2017,-2015,
	-2013,-2011,-2009,-2007,-2005,-2003,-2001,-1998,-1996,-1994,-1992,-1989,-1987,-1984,
	-1982,-1979,-1977,-1974,-1971,-1969,-1966,-1963,-1960,-1957,-1954,-1952,-1949,-1946,
	-1942,-1939,-1936,-1933,-1930,-1927,-1923,-1920,-1917,-1913,-1910,-1906,-1903,-1899,
	-1896,-1892,-1889,-1885,-1881,-1878,-1874,-1870,-1866,-1863,-1859,-1855,-1851,-1847,
	-1843,-1839,-1835,-1831,-1827,-1823,-1819,-1814,-1810,-1806,-1802,-1798,-1793,-1789,
	-1785,-1780,-1776,-1771,-1767,-1763,-1758,-1754,-1749,-1745,-1740,-1736,-1731,-1726,
	-1722,-1717,-1713,-1708,-1703,-1698,-1694,-1689,-1684,-1680,-1675,-1670,-1665,-1660,
	-1656,-1651,-1646,-1641,-1636,-1631,-1626,-1622,-1617,-1612,-1607,-1602,-1597,-1592,
	-1587,-1582,-1577,-1572,-1567,-1562,-1557,-1552,-1547,-1542,-1537,-1532,-1527,-1522,
	-1517,-1512,-1507,-1502,-1497,-1492,-1487,-1482,-1477,-1471,-1466,-1461,-1456,-1451,
	-1446,-1441,-1436,-1431,-1426,-1421,-1416,-1411,-1406,-1401,-1395,-1390,-1385,-1380,
	-1375,-1370,-1365,-1360,-1355,-1350,-1345,-1340,-1335,-1330,-1325,-1320,-1315,-1310,
	-1305,-1300,-1295,-1290,-1285,-1280,-1275,-1270,-1265,-1260,-1255,-1250,-1245,-1240,
	-1235,-1230,-1225,-1220,-1215,-1210,-1205,-1200,-1195,-1190,-1185,-1180,-1175,-1171,
	-1166,-1161,-1156,-1151,-1146,-1141,-1136,-1131,-1127,-1122,-1117,-1112,-1107,-1102,
	-1098,-1093,-1088,-1083,-1078,-1074,-1069,-1064,-1059,-1055,-1050,-1045,-1040,-1036,
	-1031,-1026,-1022,-1017,-1012,-1007,-1003,-998,-994,-989,-984,-980,-975,-970,-966,
	-961,-957,-952,-948,-943,-938,-934,-929,-925,-920,-916,-911,-907,-902,-898,-894,
	-889,-885,-880,-876,-872,-867,-863,-858,-854,-850,-845,-841,-837,-833,-828,-824,
	-820,-816,-811,-807,-803,-799,-795,-790,-786,-782,-778,-774,-770,-766,-762,-757,
	-753,-749,-745,-741,-737,-733,-729,-725,-721,-717,-714,-710,-706,-702,-698,-694,
	-690,-686,-683,-679,-675,-671,-667,-664,-660,-656,-652,-649,-645,-641,-638,-634,
	-630,-627,-623,-620,-616,-612,-609,-605,-602,-598,-595,-591,-588,-584,-581,-577,
	-574,-571,-567,-564,-560,-557,-554,-550,-547,-544,-540,-537,-534,-530,-527,-524,
	-521,-518,-514,-511,-508,-505,-502,-499,-495,-492,-489,-486,-483,-480,-477,-474,
	-471,-468,-465,-462,-459,-456,-453,-450,-447,-444,-441,-438,-435,-433,-430,-427,
	-424,-421,-418,-416,-413,-410,-407,-405,-402,-399,-396,-394,-391,-388,-386,-383,
	-380,-378,-375,-373,-370,-367,-365,-362,-360,-357,-355,-352,-350,-347,-345,-342,
	-340,-337,-335,-333,-330,-328,-325,-323,-321,-318,-316,-314,-311,-309,-307,-305,
	-302,-300,-298,-296,-293,-291,-289,-287,-285,-282,-280,-278,-276,-274,-272,-270,
	-268,-266,-264,-261,-259,-257,-255,-253,-251,-249,-247,-246,-244,-242,-240,-238,
	-236,-234,-232,-230,-228,-227,-225,-223,-221,-219,-217,-216,-214,-212,-210,-209,
	-207,-205,-203,-202,-200,-198,-197,-195,-193,-192,-190,-188,-187,-185,-184,-182,
	-180,-179,-177,-176,-174,-173,-171,-170,-168,-167,-165,-164,-162,-161,-159,-158,
	-156,-155,-154,-152,-151,-149,-148,-147,-145,-144,-143,-141,-140,-139,-137,-136,
	-135,-133,-132,-131,-130,-128,-127,-126,-125,-123,-122,-121,-120,-119,-117,-116,
	-115,-114,-113,-112,-111,-109,-108,-107,-106,-105,-104,-103,-102,-101,-100,-99,-98,
	-97,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,-86,-85,-84,-83,-82,-81,-80,-79,-78,
	-78,-77,-76,-75,-74,-73,-72,-71,-71,-70,-69,-68,-67,-67,-66,-65,-64,-63,-63,-62,
	-61,-60,-60,-59,-58,-58,-57,-56,-55,-55,-54,-53,-53,-52,-51,-51,-50,-49,-49,-48,
	-47,-47,-46,-45,-45,-44,-44,-43,-42,-42,-41,-41,-40,-40,-39,-38,-38,-37,-37,-36,
	-36,-35,-35,-34,-34,-33,-33,-32,-32,-31,-31,-30,-30,-29,-29,-28,-28,-28,-27,-27,
	-26,-26,-25,-25,-25,-24,-24,-23,-23,-23,-22,-22,-21,-21,-21,-20,-20,-20,-19,-19,
	-19,-18,-18,-17,-17,-17,-17,-16,-16,-16,-15,-15,-15,-14,-14,-14,-13,-13,-13,-13,
	-12,-12,-12,-12,-11,-11,-11,-11,-10,-10,-10,-10,-9,-9,-9,-9,-9,-8,-8,-8,-8,-7,-7,
	-7,-7,-7,-7,-6,-6,-6,-6,-6,-5,-5,-5,-5,-5,-5,-5,-4,-4,-4,-4,-4,-4,-4,-3,-3,-3,-3,
	-3,-3,-3,-3,-3,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	}
};


// we do not initialize blepState here
// cppcheck-suppress uninitMemberVar
State::State(uint32 sampleRate)
{
	double amigaClocksPerSample = static_cast<double>(PAULA_HZ) / sampleRate;
	numSteps = static_cast<int>(amigaClocksPerSample / MINIMUM_INTERVAL);
	stepRemainder = SamplePosition::FromDouble(amigaClocksPerSample - numSteps * MINIMUM_INTERVAL);
	remainder = SamplePosition(0);
	activeBleps = 0;
	globalOutputLevel = 0;
}


void State::Reset()
{
	remainder = SamplePosition(0);
	activeBleps = 0;
	globalOutputLevel = 0;
}


void State::InputSample(int16 sample)
{
	if(sample != globalOutputLevel)
	{
		LimitMax(activeBleps, static_cast<uint16>(mpt::size(blepState) - 1));

		// Make room for new blep in the sorted list
		std::move_backward(blepState, blepState + activeBleps, blepState + activeBleps + 1);

		// Start a new blep: level is the difference, age (or phase) is 0 clocks.
		activeBleps++;
		blepState[0].age = 0;
		blepState[0].level = sample - globalOutputLevel;
		globalOutputLevel = sample;
	}
}


// Return output simulated as series of bleps
int State::OutputSample(bool filter)
{
	int output = globalOutputLevel * (1 << Paula::BLEP_SCALE);
	for(uint16 i = 0; i < activeBleps; i++)
	{
		const auto &blep = blepState[i];
		output -= WinSincIntegral[filter][blep.age] * blep.level;
	}
	output /= (1 << (Paula::BLEP_SCALE - 2));	// - 2 to compensate for the fact that we reduced the input sample bit depth

	return output;
}


// Advance the simulation by given number of clock ticks
void State::Clock(int cycles)
{
	for(uint16 i = 0; i < activeBleps; i++)
	{
		blepState[i].age += static_cast<uint16>(cycles);
		if(blepState[i].age >= mpt::size(WinSincIntegral[0]))
		{
			activeBleps = i;
			return;
		}
	}
}

}

OPENMPT_NAMESPACE_END
