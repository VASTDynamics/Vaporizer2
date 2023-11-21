/*
VAST Dynamics Audio Software (TM)
*/

#include "VASTOversampler.h"
#include "../VASTEngineHeader.h"
#include "../Utils/VASTSynthfunctions.h"

CVASTOversampler::CVASTOversampler()
{
	// NOTE: Both Interpolation and Decimation filters are 
	// Optimal Method Filters designed at 176400Hz; fpass = 20kHz, fstop = 22kHz
	// pb ripple = 0.1 dB
	// sd  < -96dB
	// adjusted till ripple was minimized AND length was a multiple of OS Ratio (4) for polyphase later
	// used RackAFX FIR Designer
	// the Left IR array is used for both filters
	// 4X Coeffs
	m_nIRLength = C_FACTOR4_IR_LENGTH;

	m_h_Left[0] = 0.0000005445158422f;
	m_h_Left[1] = 0.0000005707121318f;
	m_h_Left[2] = -0.0000002019931742f;
	m_h_Left[3] = -0.0000027811349810f;
	m_h_Left[4] = -0.0000079230067058f;
	m_h_Left[5] = -0.0000157045305968f;
	m_h_Left[6] = -0.0000250798548223f;
	m_h_Left[7] = -0.0000337998826581f;
	m_h_Left[8] = -0.0000389039269066f;
	m_h_Left[9] = -0.0000377439646400f;
	m_h_Left[10] = -0.0000292139411613f;
	m_h_Left[11] = -0.0000146474785652f;
	m_h_Left[12] = 0.0000021514131276f;
	m_h_Left[13] = 0.0000159797546075f;
	m_h_Left[14] = 0.0000221549125854f;
	m_h_Left[15] = 0.0000185557037184f;
	m_h_Left[16] = 0.0000068430767897f;
	m_h_Left[17] = -0.0000078581069829f;
	m_h_Left[18] = -0.0000189057973330f;
	m_h_Left[19] = -0.0000210210473597f;
	m_h_Left[20] = -0.0000128407873490f;
	m_h_Left[21] = 0.0000021356875095f;
	m_h_Left[22] = 0.0000168605420185f;
	m_h_Left[23] = 0.0000239175660681f;
	m_h_Left[24] = 0.0000191849812836f;
	m_h_Left[25] = 0.0000041909465835f;
	m_h_Left[26] = -0.0000141687769428f;
	m_h_Left[27] = -0.0000266804745479f;
	m_h_Left[28] = -0.0000263464771706f;
	m_h_Left[29] = -0.0000122119981825f;
	m_h_Left[30] = 0.0000095742261692f;
	m_h_Left[31] = 0.0000282986147795f;
	m_h_Left[32] = 0.0000338566060236f;
	m_h_Left[33] = 0.0000221132413571f;
	m_h_Left[34] = -0.0000023518437047f;
	m_h_Left[35] = -0.0000278249426628f;
	m_h_Left[36] = -0.0000409598324040f;
	m_h_Left[37] = -0.0000336766352120f;
	m_h_Left[38] = -0.0000079384271885f;
	m_h_Left[39] = 0.0000243380400207f;
	m_h_Left[40] = 0.0000466642231913f;
	m_h_Left[41] = 0.0000463036813017f;
	m_h_Left[42] = 0.0000213879866351f;
	m_h_Left[43] = -0.0000170829262061f;
	m_h_Left[44] = -0.0000499074158142f;
	m_h_Left[45] = -0.0000591524149058f;
	m_h_Left[46] = -0.0000378185868612f;
	m_h_Left[47] = 0.0000054448237279f;
	m_h_Left[48] = 0.0000495497297379f;
	m_h_Left[49] = 0.0000711093161954f;
	m_h_Left[50] = 0.0000567109709664f;
	m_h_Left[51] = 0.0000109462253022f;
	m_h_Left[52] = -0.0000444776051154f;
	m_h_Left[53] = -0.0000808542754385f;
	m_h_Left[54] = -0.0000772124767536f;
	m_h_Left[55] = -0.0000321749212162f;
	m_h_Left[56] = 0.0000336578195856f;
	m_h_Left[57] = 0.0000868840143085f;
	m_h_Left[58] = 0.0000980972254183f;
	m_h_Left[59] = 0.0000579376974201f;
	m_h_Left[60] = -0.0000162688975252f;
	m_h_Left[61] = -0.0000876213744050f;
	m_h_Left[62] = -0.0001178134771180f;
	m_h_Left[63] = -0.0000875171608641f;
	m_h_Left[64] = -0.0000082237074821f;
	m_h_Left[65] = 0.0000814914092189f;
	m_h_Left[66] = 0.0001345096825389f;
	m_h_Left[67] = 0.0001197328674607f;
	m_h_Left[68] = 0.0000399582386308f;
	m_h_Left[69] = -0.0000670430599712f;
	m_h_Left[70] = -0.0001461128558731f;
	m_h_Left[71] = -0.0001529414148536f;
	m_h_Left[72] = -0.0000786032978795f;
	m_h_Left[73] = 0.0000430524851254f;
	m_h_Left[74] = 0.0001504003303126f;
	m_h_Left[75] = 0.0001850285043474f;
	m_h_Left[76] = 0.0001232606882695f;
	m_h_Left[77] = -0.0000086719082901f;
	m_h_Left[78] = -0.0001451331336284f;
	m_h_Left[79] = -0.0002134631940862f;
	m_h_Left[80] = -0.0001724150206428f;
	m_h_Left[81] = -0.0000364416373486f;
	m_h_Left[82] = 0.0001281958102481f;
	m_h_Left[83] = 0.0002353741147090f;
	m_h_Left[84] = 0.0002239009336336f;
	m_h_Left[85] = 0.0000919931262615f;
	m_h_Left[86] = -0.0000977704112302f;
	m_h_Left[87] = -0.0002476811932866f;
	m_h_Left[88] = -0.0002749281702563f;
	m_h_Left[89] = -0.0001569569285493f;
	m_h_Left[90] = 0.0000524965180375f;
	m_h_Left[91] = 0.0002472368068993f;
	m_h_Left[92] = 0.0003221260849386f;
	m_h_Left[93] = 0.0002294945443282f;
	m_h_Left[94] = 0.0000083545846792f;
	m_h_Left[95] = -0.0002310073177796f;
	m_h_Left[96] = -0.0003616396570578f;
	m_h_Left[97] = -0.0003069138329010f;
	m_h_Left[98] = -0.0000847206174512f;
	m_h_Left[99] = 0.0001962697569979f;
	m_h_Left[100] = 0.0003892597160302f;
	m_h_Left[101] = 0.0003856586990878f;
	m_h_Left[102] = 0.0001755936391419f;
	m_h_Left[103] = -0.0001408394600730f;
	m_h_Left[104] = -0.0004006113158539f;
	m_h_Left[105] = -0.0004613618366420f;
	m_h_Left[106] = -0.0002789076534100f;
	m_h_Left[107] = 0.0000632931623841f;
	m_h_Left[108] = 0.0003913739928976f;
	m_h_Left[109] = 0.0005289468099363f;
	m_h_Left[110] = 0.0003914636326954f;
	m_h_Left[111] = 0.0000368107284885f;
	m_h_Left[112] = -0.0003575389855541f;
	m_h_Left[113] = -0.0005827903514728f;
	m_h_Left[114] = -0.0005089115002193f;
	m_h_Left[115] = -0.0001587389997439f;
	m_h_Left[116] = 0.0002956803364214f;
	m_h_Left[117] = 0.0006169307744130f;
	m_h_Left[118] = 0.0006257825298235f;
	m_h_Left[119] = 0.0003004157624673f;
	m_h_Left[120] = -0.0002032436459558f;
	m_h_Left[121] = -0.0006253321189433f;
	m_h_Left[122] = -0.0007355912821367f;
	m_h_Left[123] = -0.0004583036352415f;
	m_h_Left[124] = 0.0000788305187598f;
	m_h_Left[125] = 0.0006021905574016f;
	m_h_Left[126] = 0.0008310037665069f;
	m_h_Left[127] = 0.0006273413309827f;
	m_h_Left[128] = 0.0000775332082412f;
	m_h_Left[129] = -0.0005422755493782f;
	m_h_Left[130] = -0.0009040769655257f;
	m_h_Left[131] = -0.0008009540033527f;
	m_h_Left[132] = -0.0002641671453603f;
	m_h_Left[133] = 0.0004412865964696f;
	m_h_Left[134] = 0.0009465577895753f;
	m_h_Left[135] = 0.0009711356833577f;
	m_h_Left[136] = 0.0004775526758749f;
	m_h_Left[137] = -0.0002962144499179f;
	m_h_Left[138] = -0.0009502378525212f;
	m_h_Left[139] = -0.0011286106891930f;
	m_h_Left[140] = -0.0007122103124857f;
	m_h_Left[141] = 0.0001056927067111f;
	m_h_Left[142] = 0.0009073557448573f;
	m_h_Left[143] = 0.0012630778364837f;
	m_h_Left[144] = 0.0009606519597583f;
	m_h_Left[145] = 0.0001296803529840f;
	m_h_Left[146] = -0.0008110329508781f;
	m_h_Left[147] = -0.0013635336654261f;
	m_h_Left[148] = -0.0012134213466197f;
	m_h_Left[149] = -0.0004070691065863f;
	m_h_Left[150] = 0.0006557236192748f;
	m_h_Left[151] = 0.0014186699409038f;
	m_h_Left[152] = 0.0014592278748751f;
	m_h_Left[153] = 0.0007211973425001f;
	m_h_Left[154] = -0.0004376557189971f;
	m_h_Left[155] = -0.0014173235977069f;
	m_h_Left[156] = -0.0016851695254445f;
	m_h_Left[157] = -0.0010642196284607f;
	m_h_Left[158] = 0.0001552503381390f;
	m_h_Left[159] = 0.0013489727862179f;
	m_h_Left[160] = 0.0018770446768031f;
	m_h_Left[161] = 0.0014256818685681f;
	m_h_Left[162] = 0.0001905011304189f;
	m_h_Left[163] = -0.0012042585294694f;
	m_h_Left[164] = -0.0020197455305606f;
	m_h_Left[165] = -0.0017925744177774f;
	m_h_Left[166] = -0.0005957146058790f;
	m_h_Left[167] = 0.0009755205828696f;
	m_h_Left[168] = 0.0020977298263460f;
	m_h_Left[169] = 0.0021494934335351f;
	m_h_Left[170] = 0.0010533761233091f;
	m_h_Left[171] = -0.0006573111750185f;
	m_h_Left[172] = -0.0020955423824489f;
	m_h_Left[173] = -0.0024788931477815f;
	m_h_Left[174] = -0.0015531908720732f;
	m_h_Left[175] = 0.0002468732418492f;
	m_h_Left[176] = 0.0019983709789813f;
	m_h_Left[177] = 0.0027614291757345f;
	m_h_Left[178] = 0.0020815343596041f;
	m_h_Left[179] = 0.0002554488019086f;
	m_h_Left[180] = -0.0017925972351804f;
	m_h_Left[181] = -0.0029763551428914f;
	m_h_Left[182] = -0.0026214842218906f;
	m_h_Left[183] = -0.0008458612719551f;
	m_h_Left[184] = 0.0014663392212242f;
	m_h_Left[185] = 0.0031019740272313f;
	m_h_Left[186] = 0.0031529506668448f;
	m_h_Left[187] = 0.0015168727841228f;
	m_h_Left[188] = -0.0010099314386025f;
	m_h_Left[189] = -0.0031160889193416f;
	m_h_Left[190] = -0.0036528608761728f;
	m_h_Left[191] = -0.0022571331355721f;
	m_h_Left[192] = 0.0004163304984104f;
	m_h_Left[193] = 0.0029964295681566f;
	m_h_Left[194] = 0.0040953969582915f;
	m_h_Left[195] = 0.0030513887759298f;
	m_h_Left[196] = 0.0003186264366377f;
	m_h_Left[197] = -0.0027209594845772f;
	m_h_Left[198] = -0.0044521889649332f;
	m_h_Left[199] = -0.0038805003277957f;
	m_h_Left[200] = -0.0011961093405262f;
	m_h_Left[201] = 0.0022680191323161f;
	m_h_Left[202] = 0.0046924264170229f;
	m_h_Left[203] = 0.0047215311788023f;
	m_h_Left[204] = 0.0022144161630422f;
	m_h_Left[205] = -0.0016161559615284f;
	m_h_Left[206] = -0.0047827241942286f;
	m_h_Left[207] = -0.0055478112772107f;
	m_h_Left[208] = -0.0033693523146212f;
	m_h_Left[209] = 0.0007435118313879f;
	m_h_Left[210] = 0.0046866005286574f;
	m_h_Left[211] = 0.0063289487734437f;
	m_h_Left[212] = 0.0046550347469747f;
	m_h_Left[213] = 0.0003735880600289f;
	m_h_Left[214] = -0.0043631680309772f;
	m_h_Left[215] = -0.0070305466651917f;
	m_h_Left[216] = -0.0060652270913124f;
	m_h_Left[217] = -0.0017632801318541f;
	m_h_Left[218] = 0.0037645003758371f;
	m_h_Left[219] = 0.0076133953407407f;
	m_h_Left[220] = 0.0075955823995173f;
	m_h_Left[221] = 0.0034630466252565f;
	m_h_Left[222] = -0.0028304771985859f;
	m_h_Left[223] = -0.0080314734950662f;
	m_h_Left[224] = -0.0092473234981298f;
	m_h_Left[225] = -0.0055286134593189f;
	m_h_Left[226] = 0.0014789294218645f;
	m_h_Left[227] = 0.0082277162000537f;
	m_h_Left[228] = 0.0110338740050793f;
	m_h_Left[229] = 0.0080511523410678f;
	m_h_Left[230] = 0.0004142033576500f;
	m_h_Left[231] = -0.0081247519701719f;
	m_h_Left[232] = -0.0129936235025525f;
	m_h_Left[233] = -0.0111927380785346f;
	m_h_Left[234] = -0.0030573662370443f;
	m_h_Left[235] = 0.0076039703562856f;
	m_h_Left[236] = 0.0152177270501852f;
	m_h_Left[237] = 0.0152677698060870f;
	m_h_Left[238] = 0.0068449787795544f;
	m_h_Left[239] = -0.0064526787027717f;
	m_h_Left[240] = -0.0179194156080484f;
	m_h_Left[241] = -0.0209582298994064f;
	m_h_Left[242] = -0.0126397209241986f;
	m_h_Left[243] = 0.0042087305337191f;
	m_h_Left[244] = 0.0216464996337891f;
	m_h_Left[245] = 0.0300200320780277f;
	m_h_Left[246] = 0.0227684658020735f;
	m_h_Left[247] = 0.0004358185979072f;
	m_h_Left[248] = -0.0281703099608421f;
	m_h_Left[249] = -0.0485937669873238f;
	m_h_Left[250] = -0.0463617891073227f;
	m_h_Left[251] = -0.0134116141125560f;
	m_h_Left[252] = 0.0474678650498390f;
	m_h_Left[253] = 0.1222959980368614f;
	m_h_Left[254] = 0.1901284903287888f;
	m_h_Left[255] = 0.2303789854049683f;
	m_h_Left[256] = 0.2303789854049683f;
	m_h_Left[257] = 0.1901284903287888f;
	m_h_Left[258] = 0.1222959980368614f;
	m_h_Left[259] = 0.0474678650498390f;
	m_h_Left[260] = -0.0134116141125560f;
	m_h_Left[261] = -0.0463617891073227f;
	m_h_Left[262] = -0.0485937669873238f;
	m_h_Left[263] = -0.0281703099608421f;
	m_h_Left[264] = 0.0004358185979072f;
	m_h_Left[265] = 0.0227684658020735f;
	m_h_Left[266] = 0.0300200320780277f;
	m_h_Left[267] = 0.0216464996337891f;
	m_h_Left[268] = 0.0042087305337191f;
	m_h_Left[269] = -0.0126397209241986f;
	m_h_Left[270] = -0.0209582298994064f;
	m_h_Left[271] = -0.0179194156080484f;
	m_h_Left[272] = -0.0064526787027717f;
	m_h_Left[273] = 0.0068449787795544f;
	m_h_Left[274] = 0.0152677698060870f;
	m_h_Left[275] = 0.0152177270501852f;
	m_h_Left[276] = 0.0076039703562856f;
	m_h_Left[277] = -0.0030573662370443f;
	m_h_Left[278] = -0.0111927380785346f;
	m_h_Left[279] = -0.0129936235025525f;
	m_h_Left[280] = -0.0081247519701719f;
	m_h_Left[281] = 0.0004142033576500f;
	m_h_Left[282] = 0.0080511523410678f;
	m_h_Left[283] = 0.0110338740050793f;
	m_h_Left[284] = 0.0082277162000537f;
	m_h_Left[285] = 0.0014789294218645f;
	m_h_Left[286] = -0.0055286134593189f;
	m_h_Left[287] = -0.0092473234981298f;
	m_h_Left[288] = -0.0080314734950662f;
	m_h_Left[289] = -0.0028304771985859f;
	m_h_Left[290] = 0.0034630466252565f;
	m_h_Left[291] = 0.0075955823995173f;
	m_h_Left[292] = 0.0076133953407407f;
	m_h_Left[293] = 0.0037645003758371f;
	m_h_Left[294] = -0.0017632801318541f;
	m_h_Left[295] = -0.0060652270913124f;
	m_h_Left[296] = -0.0070305466651917f;
	m_h_Left[297] = -0.0043631680309772f;
	m_h_Left[298] = 0.0003735880600289f;
	m_h_Left[299] = 0.0046550347469747f;
	m_h_Left[300] = 0.0063289487734437f;
	m_h_Left[301] = 0.0046866005286574f;
	m_h_Left[302] = 0.0007435118313879f;
	m_h_Left[303] = -0.0033693523146212f;
	m_h_Left[304] = -0.0055478112772107f;
	m_h_Left[305] = -0.0047827241942286f;
	m_h_Left[306] = -0.0016161559615284f;
	m_h_Left[307] = 0.0022144161630422f;
	m_h_Left[308] = 0.0047215311788023f;
	m_h_Left[309] = 0.0046924264170229f;
	m_h_Left[310] = 0.0022680191323161f;
	m_h_Left[311] = -0.0011961093405262f;
	m_h_Left[312] = -0.0038805003277957f;
	m_h_Left[313] = -0.0044521889649332f;
	m_h_Left[314] = -0.0027209594845772f;
	m_h_Left[315] = 0.0003186264366377f;
	m_h_Left[316] = 0.0030513887759298f;
	m_h_Left[317] = 0.0040953969582915f;
	m_h_Left[318] = 0.0029964295681566f;
	m_h_Left[319] = 0.0004163304984104f;
	m_h_Left[320] = -0.0022571331355721f;
	m_h_Left[321] = -0.0036528608761728f;
	m_h_Left[322] = -0.0031160889193416f;
	m_h_Left[323] = -0.0010099314386025f;
	m_h_Left[324] = 0.0015168727841228f;
	m_h_Left[325] = 0.0031529506668448f;
	m_h_Left[326] = 0.0031019740272313f;
	m_h_Left[327] = 0.0014663392212242f;
	m_h_Left[328] = -0.0008458612719551f;
	m_h_Left[329] = -0.0026214842218906f;
	m_h_Left[330] = -0.0029763551428914f;
	m_h_Left[331] = -0.0017925972351804f;
	m_h_Left[332] = 0.0002554488019086f;
	m_h_Left[333] = 0.0020815343596041f;
	m_h_Left[334] = 0.0027614291757345f;
	m_h_Left[335] = 0.0019983709789813f;
	m_h_Left[336] = 0.0002468732418492f;
	m_h_Left[337] = -0.0015531908720732f;
	m_h_Left[338] = -0.0024788931477815f;
	m_h_Left[339] = -0.0020955423824489f;
	m_h_Left[340] = -0.0006573111750185f;
	m_h_Left[341] = 0.0010533761233091f;
	m_h_Left[342] = 0.0021494934335351f;
	m_h_Left[343] = 0.0020977298263460f;
	m_h_Left[344] = 0.0009755205828696f;
	m_h_Left[345] = -0.0005957146058790f;
	m_h_Left[346] = -0.0017925744177774f;
	m_h_Left[347] = -0.0020197455305606f;
	m_h_Left[348] = -0.0012042585294694f;
	m_h_Left[349] = 0.0001905011304189f;
	m_h_Left[350] = 0.0014256818685681f;
	m_h_Left[351] = 0.0018770446768031f;
	m_h_Left[352] = 0.0013489727862179f;
	m_h_Left[353] = 0.0001552503381390f;
	m_h_Left[354] = -0.0010642196284607f;
	m_h_Left[355] = -0.0016851695254445f;
	m_h_Left[356] = -0.0014173235977069f;
	m_h_Left[357] = -0.0004376557189971f;
	m_h_Left[358] = 0.0007211973425001f;
	m_h_Left[359] = 0.0014592278748751f;
	m_h_Left[360] = 0.0014186699409038f;
	m_h_Left[361] = 0.0006557236192748f;
	m_h_Left[362] = -0.0004070691065863f;
	m_h_Left[363] = -0.0012134213466197f;
	m_h_Left[364] = -0.0013635336654261f;
	m_h_Left[365] = -0.0008110329508781f;
	m_h_Left[366] = 0.0001296803529840f;
	m_h_Left[367] = 0.0009606519597583f;
	m_h_Left[368] = 0.0012630778364837f;
	m_h_Left[369] = 0.0009073557448573f;
	m_h_Left[370] = 0.0001056927067111f;
	m_h_Left[371] = -0.0007122103124857f;
	m_h_Left[372] = -0.0011286106891930f;
	m_h_Left[373] = -0.0009502378525212f;
	m_h_Left[374] = -0.0002962144499179f;
	m_h_Left[375] = 0.0004775526758749f;
	m_h_Left[376] = 0.0009711356833577f;
	m_h_Left[377] = 0.0009465577895753f;
	m_h_Left[378] = 0.0004412865964696f;
	m_h_Left[379] = -0.0002641671453603f;
	m_h_Left[380] = -0.0008009540033527f;
	m_h_Left[381] = -0.0009040769655257f;
	m_h_Left[382] = -0.0005422755493782f;
	m_h_Left[383] = 0.0000775332082412f;
	m_h_Left[384] = 0.0006273413309827f;
	m_h_Left[385] = 0.0008310037665069f;
	m_h_Left[386] = 0.0006021905574016f;
	m_h_Left[387] = 0.0000788305187598f;
	m_h_Left[388] = -0.0004583036352415f;
	m_h_Left[389] = -0.0007355912821367f;
	m_h_Left[390] = -0.0006253321189433f;
	m_h_Left[391] = -0.0002032436459558f;
	m_h_Left[392] = 0.0003004157624673f;
	m_h_Left[393] = 0.0006257825298235f;
	m_h_Left[394] = 0.0006169307744130f;
	m_h_Left[395] = 0.0002956803364214f;
	m_h_Left[396] = -0.0001587389997439f;
	m_h_Left[397] = -0.0005089115002193f;
	m_h_Left[398] = -0.0005827903514728f;
	m_h_Left[399] = -0.0003575389855541f;
	m_h_Left[400] = 0.0000368107284885f;
	m_h_Left[401] = 0.0003914636326954f;
	m_h_Left[402] = 0.0005289468099363f;
	m_h_Left[403] = 0.0003913739928976f;
	m_h_Left[404] = 0.0000632931623841f;
	m_h_Left[405] = -0.0002789076534100f;
	m_h_Left[406] = -0.0004613618366420f;
	m_h_Left[407] = -0.0004006113158539f;
	m_h_Left[408] = -0.0001408394600730f;
	m_h_Left[409] = 0.0001755936391419f;
	m_h_Left[410] = 0.0003856586990878f;
	m_h_Left[411] = 0.0003892597160302f;
	m_h_Left[412] = 0.0001962697569979f;
	m_h_Left[413] = -0.0000847206174512f;
	m_h_Left[414] = -0.0003069138329010f;
	m_h_Left[415] = -0.0003616396570578f;
	m_h_Left[416] = -0.0002310073177796f;
	m_h_Left[417] = 0.0000083545846792f;
	m_h_Left[418] = 0.0002294945443282f;
	m_h_Left[419] = 0.0003221260849386f;
	m_h_Left[420] = 0.0002472368068993f;
	m_h_Left[421] = 0.0000524965180375f;
	m_h_Left[422] = -0.0001569569285493f;
	m_h_Left[423] = -0.0002749281702563f;
	m_h_Left[424] = -0.0002476811932866f;
	m_h_Left[425] = -0.0000977704112302f;
	m_h_Left[426] = 0.0000919931262615f;
	m_h_Left[427] = 0.0002239009336336f;
	m_h_Left[428] = 0.0002353741147090f;
	m_h_Left[429] = 0.0001281958102481f;
	m_h_Left[430] = -0.0000364416373486f;
	m_h_Left[431] = -0.0001724150206428f;
	m_h_Left[432] = -0.0002134631940862f;
	m_h_Left[433] = -0.0001451331336284f;
	m_h_Left[434] = -0.0000086719082901f;
	m_h_Left[435] = 0.0001232606882695f;
	m_h_Left[436] = 0.0001850285043474f;
	m_h_Left[437] = 0.0001504003303126f;
	m_h_Left[438] = 0.0000430524851254f;
	m_h_Left[439] = -0.0000786032978795f;
	m_h_Left[440] = -0.0001529414148536f;
	m_h_Left[441] = -0.0001461128558731f;
	m_h_Left[442] = -0.0000670430599712f;
	m_h_Left[443] = 0.0000399582386308f;
	m_h_Left[444] = 0.0001197328674607f;
	m_h_Left[445] = 0.0001345096825389f;
	m_h_Left[446] = 0.0000814914092189f;
	m_h_Left[447] = -0.0000082237074821f;
	m_h_Left[448] = -0.0000875171608641f;
	m_h_Left[449] = -0.0001178134771180f;
	m_h_Left[450] = -0.0000876213744050f;
	m_h_Left[451] = -0.0000162688975252f;
	m_h_Left[452] = 0.0000579376974201f;
	m_h_Left[453] = 0.0000980972254183f;
	m_h_Left[454] = 0.0000868840143085f;
	m_h_Left[455] = 0.0000336578195856f;
	m_h_Left[456] = -0.0000321749212162f;
	m_h_Left[457] = -0.0000772124767536f;
	m_h_Left[458] = -0.0000808542754385f;
	m_h_Left[459] = -0.0000444776051154f;
	m_h_Left[460] = 0.0000109462253022f;
	m_h_Left[461] = 0.0000567109709664f;
	m_h_Left[462] = 0.0000711093161954f;
	m_h_Left[463] = 0.0000495497297379f;
	m_h_Left[464] = 0.0000054448237279f;
	m_h_Left[465] = -0.0000378185868612f;
	m_h_Left[466] = -0.0000591524149058f;
	m_h_Left[467] = -0.0000499074158142f;
	m_h_Left[468] = -0.0000170829262061f;
	m_h_Left[469] = 0.0000213879866351f;
	m_h_Left[470] = 0.0000463036813017f;
	m_h_Left[471] = 0.0000466642231913f;
	m_h_Left[472] = 0.0000243380400207f;
	m_h_Left[473] = -0.0000079384271885f;
	m_h_Left[474] = -0.0000336766352120f;
	m_h_Left[475] = -0.0000409598324040f;
	m_h_Left[476] = -0.0000278249426628f;
	m_h_Left[477] = -0.0000023518437047f;
	m_h_Left[478] = 0.0000221132413571f;
	m_h_Left[479] = 0.0000338566060236f;
	m_h_Left[480] = 0.0000282986147795f;
	m_h_Left[481] = 0.0000095742261692f;
	m_h_Left[482] = -0.0000122119981825f;
	m_h_Left[483] = -0.0000263464771706f;
	m_h_Left[484] = -0.0000266804745479f;
	m_h_Left[485] = -0.0000141687769428f;
	m_h_Left[486] = 0.0000041909465835f;
	m_h_Left[487] = 0.0000191849812836f;
	m_h_Left[488] = 0.0000239175660681f;
	m_h_Left[489] = 0.0000168605420185f;
	m_h_Left[490] = 0.0000021356875095f;
	m_h_Left[491] = -0.0000128407873490f;
	m_h_Left[492] = -0.0000210210473597f;
	m_h_Left[493] = -0.0000189057973330f;
	m_h_Left[494] = -0.0000078581069829f;
	m_h_Left[495] = 0.0000068430767897f;
	m_h_Left[496] = 0.0000185557037184f;
	m_h_Left[497] = 0.0000221549125854f;
	m_h_Left[498] = 0.0000159797546075f;
	m_h_Left[499] = 0.0000021514131276f;
	m_h_Left[500] = -0.0000146474785652f;
	m_h_Left[501] = -0.0000292139411613f;
	m_h_Left[502] = -0.0000377439646400f;
	m_h_Left[503] = -0.0000389039269066f;
	m_h_Left[504] = -0.0000337998826581f;
	m_h_Left[505] = -0.0000250798548223f;
	m_h_Left[506] = -0.0000157045305968f;
	m_h_Left[507] = -0.0000079230067058f;
	m_h_Left[508] = -0.0000027811349810f;
	m_h_Left[509] = -0.0000002019931742f;
	m_h_Left[510] = 0.0000005707121318f;
	m_h_Left[511] = 0.0000005445158422f;

}

CVASTOversampler::~CVASTOversampler(void)
{
}

bool CVASTOversampler::init(CVASTSettings &set) {
	m_Set = &set;
	
	m_nOversamplingRatioLocal = 1;

	//m_Interpolator.init(m_nOversamplingRatioLocal, m_nIRLength, &m_h_Left[0]);
	m_Decimator.init(m_nOversamplingRatioLocal, m_nIRLength, &m_h_Left[0]);

	// flush interp buffers
	memset(m_pLeftInterpBuffer, 0, C_OVERSAMPLING_RATIO*sizeof(float));
	memset(m_pRightInterpBuffer, 0, C_OVERSAMPLING_RATIO*sizeof(float));

	// flush deci buffers
	memset(m_pLeftDecipBuffer, 0, C_OVERSAMPLING_RATIO*sizeof(float));
	memset(m_pRightDeciBuffer, 0, C_OVERSAMPLING_RATIO*sizeof(float));

	return true;
}

bool CVASTOversampler::prepareForPlay()
{
	m_nOversamplingRatioLocal = C_OVERSAMPLING_RATIO;
	//m_Interpolator.reset();
	m_Decimator.reset();
	return true;
}

void CVASTOversampler::updateVariables() {
}

//THIS HAS TO BE SSE OPTIMZED! TODO

void CVASTOversampler::upsampleAudioBuffer4(dsp::AudioBlock<float> inBlock, dsp::AudioBlock<float> outBlock, const int numSamples) {
	//int i_out = 0;
	int i_in = 0;

	int iFrames = numSamples;
	float* inl = inBlock.getChannelPointer(0);
	float* inr = inBlock.getChannelPointer(1);
	float* outl = outBlock.getChannelPointer(0);
	float* outr = outBlock.getChannelPointer(1);

	while (--iFrames >= 0) {
		int s1 = i_in * m_nOversamplingRatioLocal;
		outl[s1] = inl[i_in];
		outl[s1 + 1] = inl[i_in];
		outl[s1 + 2] = inl[i_in];
		outl[s1 + 3] = inl[i_in];
		outr[s1] = inr[i_in];
		outr[s1 + 1] = inr[i_in];
		outr[s1 + 2] = inr[i_in];
		outr[s1 + 3] = inr[i_in];
		/*
		outBlock.getSingleChannelBlock(0).getSubBlock(i_in * m_nOversamplingRatioLocal, m_nOversamplingRatioLocal).fill(inBlock.getChannelPointer(0)[i_in]);
		outBlock.getSingleChannelBlock(1).getSubBlock(i_in * m_nOversamplingRatioLocal, m_nOversamplingRatioLocal).fill(inBlock.getChannelPointer(1)[i_in]);
		*/
			/*
		for (int i = 0; i < m_nOversamplingRatioLocal; i++) {
			
			outBlock.getChannelPointer(0)[i_out] = inBlock.getChannelPointer(0)[i_in]; //check if can be optimize with buffer functions
			outBlock.getChannelPointer(1)[i_out] = inBlock.getChannelPointer(1)[i_in]; //check if can be optimize with buffer functions
			i_out++; 
		}
		*/
		i_in++;
	}
}

void CVASTOversampler::downsampleAudioBuffer4(dsp::AudioBlock<float> inBlock, dsp::AudioBlock<float> outBlock, const int numSamples) {
	int i_out = 0;
	int i_in = 0;

	bool isIdle = false;

	juce::Range<float> r = inBlock.findMinAndMax();
	if ((r.getStart() == 0.0f) && (r.getEnd() == 0.0f))
		isIdle = true;

	/*
	if (inBlock.hasBeenCleared() == true)
		isIdle = true;
	else 
		if (checkFloatBufferZero(inBlock) == true)
			isIdle = true;
	*/

	int iFrames = numSamples / m_nOversamplingRatioLocal;
	for (int j = 0; j < iFrames; j++)  {
		if ((m_nOversamplingRatioLocal == 1) || (isIdle == true))  {
			outBlock.getChannelPointer(0)[i_out] = inBlock.getChannelPointer(0)[i_in]; //check if can be optimized with buffer functions
			outBlock.getChannelPointer(1)[i_out] = inBlock.getChannelPointer(1)[i_in]; //check if can be optimized with buffer functions
			i_in++;
		}
		else
		{
			for (int i = 0; i < m_nOversamplingRatioLocal; i++) {
				// load decimation buffers with processed audio
				m_pLeftDecipBuffer[i] = inBlock.getChannelPointer(0)[i_in];
				m_pRightDeciBuffer[i] = inBlock.getChannelPointer(1)[i_in];
				i_in++;
			}
			float outL = 0.0f;
			float outR = 0.0f;
			if (m_nOversamplingRatioLocal != 1) m_Decimator.decimateSamples4(m_pLeftDecipBuffer, m_pRightDeciBuffer, outL, outR);
			
			outBlock.getChannelPointer(0)[i_out] = outL;
			outBlock.getChannelPointer(1)[i_out] = outR;
		}
		i_out++;
	}
}
