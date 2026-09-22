import json
import csv
import os
import re

# Raw text extracted from all 17 pages of the Microsoft Blocked Driver List PDF document
raw_data = """
1 1.sys Hash deny 2 1 1 0
2 2.sys Hash deny 2 1 1 0
3 32-bit dell dbutil.sys Hash deny 4 1 1 2
4 64-bit dell dbutil.sys Hash deny 4 1 1 2
5 80.sys Hash deny 2 1 1 0
6 81.sys Hash deny 6 3 3 0
7 Agent64\\05f052_4045ae_694848_8cb62c_b1d962 Hash deny 4 1 1 2
8 AMD AODDriver\\478bcb750017cb6541f3dd0d08a47370f3c92eec998bc3825b5d8e08ee831b70 Hash deny 4 1 1 2
9 AMD AODDriver\\5daa8fa3b5db2e6225a2effea41af95fe7ffc579550c4081c8028ed33bc023b8 Hash deny 4 1 1 2
10 AMD PDFWKRNL.sys\\0cf84400c09582ee2911a5b1582332c992d1cd29fcf811cb1dc00fcd61757db0 Hash deny 1 0 0 0
11 AMD PDFWKRNL.sys\\2b29b91f9f63b65e8f0ec30442a89c9304b9eefa Hash deny 1 0 0 0
12 AMD PDFWKRNL.sys\\531ae2d8f7aa301b74a37b82b5f3cadbf91962e0 Hash deny 1 0 0 0
13 AMD PDFWKRNL.sys\\57612842efbca98673e68cdbe0461d341379bfc8 Hash deny 1 0 0 0
14 AMD PDFWKRNL.sys\\f501dd79e0b49ab76bd8d43a79da292c5224fa2b Hash deny 1 0 0 0
15 amifldrv64\\26ba58c9af9c8a7aebf222f491f786daa0626be44d34f170fea3623d92828e63 Hash deny 4 1 1 2
16 amifldrv64\\2f60536b25ba8c9014e4a57d7a9a681bd3189fa414eea88c256d029750e15cae Hash deny 4 1 1 2
17 amifldrv64\\42579a759f3f95f20a2c51d5ac2047a2662a2675b3fb9f46c1ed7f23393a0f00 Hash deny 4 1 1 2
18 amifldrv64\\65c26276cadda7a36f8977d1d01120edb5c3418be2317d501761092d5f9916c9 Hash deny 4 1 1 2
19 amifldrv64\\6c64688444d3e004da77dcfb769d064bb38afceeef7ff915dfc71e60e19ff18a Hash deny 4 1 1 2
20 amifldrv64\\7c942801884999057aabdc01707570371afdb077979ee2f318c05276123b78e7 Hash deny 4 1 1 2
21 amifldrv64\\990165725debccea7ca15aa4ed7a0e3a2a25b4a72cb309a27c899bd0e4b5148f Hash deny 4 1 1 2
22 amifldrv64\\b95b2d9b29bd25659f1c7ba5a187f8d23cde01162d9b5b1a2c4aea8f64b38441 Hash deny 4 1 1 2
23 amifldrv64\\bc7ebd191e0991fd0865a5c956a92e63792a0bb2ff888af43f7a63bb65a22248 Hash deny 4 1 1 2
24 amifldrv64\\c2fcc0fec64d5647813b84b9049d430406c4c6a7b9f8b725da21bcae2ff12247 Hash deny 4 1 1 2
25 amifldrv64\\f06fdfe50ebc8d1d2daf5811b66288563f26a09a2ec9c2a21e2a71ff19756062 Hash deny 4 1 1 2
26 amifldrv64\\fc22977ff721b3d718b71c42440ee2d8a144f3fbc7755e4331ddd5bcc65158d2 Hash deny 4 1 1 2
27 amifldrv64\\fda506e2aa85dc41a4cbc23d3ecc71ab34e06f1def736e58862dc449acbc2330 Hash deny 4 1 1 2
28 ASIO32.sys Hash deny 12 3 3 6
29 asio3\\03a1e1037ea162020e75e37be771f620c4569e9621175f672583705f3ab569f7 Hash deny 2 1 1 0
30 asio3\\1ed43e18f78413774c83660106154be8bfb5f2f9809d9b369a4211365a1685c1 Hash deny 2 1 1 0
31 asio3\\218de0c801b70d8bebf0233f796de07842b84b899c49f7a7be1c0423a158b786 Hash deny 2 1 1 0
32 asio3\\383df3b803ea69e16de314c82c2099283e746a6865cc4488ac927510ab5ada9c Hash deny 2 1 1 0
33 asio3\\4d71d5e711ced1d8b215aeee8238b12908000e3d5c0d7c30927219fdb4098ab1 Hash deny 2 1 1 0
34 asio3\\69dc4782060178762305b17c353efc1508a2a6edd9baf124286f380f51f5daed Hash deny 2 1 1 0
35 asio3\\82ada50f0001fdc2b5eae4e6343582d443343400442669f73bb660bb24db1e12 Hash deny 2 1 1 0
36 asio3\\cc442919294d81400674bb9e1b68ce8f9e5aafed7f45d548862ce167d487d59f Hash deny 2 1 1 0
37 asio3\\cc58e64e2a9b77eebf466229df4c3c1f5305d6c69e50c6dd140661490ce70f1f Hash deny 2 1 1 0
38 asio3\\fa875178ae2d7604d027510b0d0a7e2d9d675e10a4c9dda2d927ee891e0bcb91 Hash deny 2 1 1 0
39 ASIO64.sys Hash deny 12 3 3 6
40 asio\\2da330a2088409efc351118445a824f11edbe51cf3d653b298053785097fe40e Hash deny 4 1 1 2
41 asio\\923ebbe8111e73d5b8ecc2db10f8ea2629a3264c3a535d01c3c118a3b4c91782 Hash deny 2 1 1 0
42 AsrDrv10.sys Hash deny 4 1 1 2
43 AsrDrv101.sys Hash deny 4 1 1 2
44 AsrDrv102.sys Hash deny 4 1 1 2
45 AsrDrv103.sys Hash deny 4 1 1 2
46 asrdrv104\\4bf974f5d3489638a48ee508b4a8cfa0f0262909778ccdd2e871172b71654d89 Hash deny 4 1 1 2
47 asrdrv104\\53bb076e81f6104f41bc284eedae36bd99b53e42719573fa5960932720ebc854 Hash deny 4 1 1 2
48 asrdrv104\\6ed35f310c96920a271c59a097b382da07856e40179c2a4239f8daa04eef38e7 Hash deny 2 1 1 0
49 asrdrv104\\d20d8bf80017e98b6dfc9f6c3960271fa792a908758bef49a390e2692a2a4341 Hash deny 4 1 1 2
50 AsrDrv105.sys\\bc3f73f643b8d3108661fe1ff6a816cbc4482a056ef90c29042b448a45077bd3 Hash deny 1 0 0 0
51 AsrDrv105n.sys\\e7477a759f3f976d8662aab9d4f4b110d8db135bbcd712bea391b81336dc856eb Hash deny 1 0 0 0
52 AsrDrv106.sys\\3943a796cc7c5352aa57ccf544295bfd6fb69aae147bc8235a00202dc6ed6838 Hash deny 1 0 0 0
53 AsrDrv106n.sys\\14c5576eda5a28d476a93c9cdb91236f18573dbd0578ac2bb183dfd115f4a166 Hash deny 1 0 0 0
54 AsrDrv107.sys\\be131e30464c7be03bd2d16f99ea2b04c106b482cc5c52be659e4c0301206348 Hash deny 1 0 0 0
55 AsrDrv107n.sys\\12177d777345f60a579e7bd8f0df95296af6e293e5560ee544fcced99a5db0df Hash deny 1 0 0 0
56 AsrSetupDrv103\\9d9346e6f46f831e263385a9bd32428e01919cca26a035bbb8e9cb00bf410bc3 Hash deny 4 1 1 2
57 AsrSetupDrv103\\a0728184caead84f2e88777d833765f2d8af6a20aad77b426e07e76ef91f5c3f Hash deny 4 1 1 2
58 AsUpIO64.sys Hash deny 4 1 1 2
59 Asus EIO64\\b17507a3246020fa0052a172485d7b3567e0161747927f2edf27c40e310852e0 Hash deny 4 1 1 2
60 Asus EIO64\\cf69704755ec2643dfd245ae1d4e15d77f306aeb1a576ffa159453de1a7345cb Hash deny 4 1 1 2
61 atillk64\\11a9787831ac4f0657aeb5e7019c23acc39d8833faf28f85bd10d7590ea4cc5f Hash deny 4 1 1 2
62 atillk64\\248dcc72d799d350d30b0f9e9ae93389cdcd11b43e38949ba9be414400657587 Hash deny 2 1 1 0
63 atillk64\\321cc3f24a518c70fb537ee9472b1777d05727c649d5b6538082a971c40ddcbe Hash deny 4 1 1 2
64 atillk64\\4780da56667e01cdd7eff83c23c772d68deb4d9fdb69d5302f556bb424151f51 Hash deny 2 1 1 0
65 atillk64\\61580186311f6260c6de7fa5bf9242d74687aa1c5c9fdf9d9a48eb46d67d636f Hash deny 2 1 1 0
66 atillk64\\6c6c5e35accc37c928d721c800476ccf4c4b5b06a1b0906dc5ff4df71ff50943 Hash deny 4 1 1 2
67 atillk64\\83ffcfaf429c8368194d7b73f7729d97d6a3b80fb203d57055f3e4eec8228914 Hash deny 2 1 1 0
68 atillk64\\be66f3bbfed7d648cfd110853ddb8cef561f94a45405afc6be06e846b697d2b0 Hash deny 2 1 1 0
69 atillk64\\c825a47817399e988912bb75106befaefae0babc0743a7e32b46f17469c78cad Hash deny 4 1 1 2
70 atillk64\\d2182b6ef3255c7c1a69223cd3c2d68eb8ba3112ce433cd49cd803dc76412d4b Hash deny 4 1 1 2
71 b.sys Hash deny 2 1 1 0
72 b1.sys Hash deny 2 1 1 0
73 b3.sys Hash deny 2 1 1 0
74 b4.sys Hash deny 2 1 1 0
75 bandainamcoonline.sys\\7ec93f34eb323823eb199fbf8d06219086d517d0e8f4b9e348d7afd41ec9fd5d Hash deny 4 1 1 2
76 BEDaisy.sys\\0a9b608461d55815e99700607a52fbdb7d598f968126d38e10cc4293ac4b1ad8 Hash deny 4 1 1 2
77 BEDaisy.sys\\0bd164da36bd637bb76ca66602d732af912bd9299cb3d520d26db528cb54826d Hash deny 4 1 1 2
78 BEDaisy.sys\\2b120de80a5462f8395cfb7153c86dfd44f29f0776ea156ec4a34fa64e5c4797 Hash deny 4 1 1 2
79 BEDaisy.sys\\3b19a7207a55d752db1b366b1dea2fd2c7620a825a3f0dcffca10af76611118c Hash deny 4 1 1 2
80 BEDaisy.sys\\3d8cfc9abea6d83dfea6da03260ff81be3b7b304321274f696ff0fdb9920c645 Hash deny 4 1 1 2
81 BEDaisy.sys\\4ba224af60a50cad10d0091c89134c72fc021da8d34a6f25c4827184dc6ca5c7 Hash deny 4 1 1 2
82 BEDaisy.sys\\5d7bfe05792189eaf7193bee85f0c792c33315cfcb40b2e62cc7baef6cafbc5c Hash deny 4 1 1 2
83 BEDaisy.sys\\7108613244f16c2279c3c917aa49cef8acf0b92fdaa9ace19bf5cf634360d727 Hash deny 4 1 1 2
84 BEDaisy.sys\\773999db2f07c50aad70e50c1983fa95804369d25a5b4f10bd610f864c27f2fc Hash deny 4 1 1 2
85 BEDaisy.sys\\7c830ed39c9de8fe711632bf44846615f84b10db383f47b7d7c9db29a2bd829a Hash deny 4 1 1 2
86 BEDaisy.sys\\854bc946b557ed78c7d40547eb39e293e83942a693c94d0e798d1c4fbde7efa9 Hash deny 4 1 1 2
87 BEDaisy.sys\\8ae383546761069b26826dfbf2ac0233169d155bca6a94160488092b4e70b222 Hash deny 4 1 1 2
88 BEDaisy.sys\\8bf01cd6d55502838853851703eb297ec71361fa9a0b088a30c2434f4d2bf9c6 Hash deny 4 1 1 2
89 BEDaisy.sys\\9bd8b0289955a6eb791f45c3203f08a64cbd457fd1b9d598a6fbbca5d0372e36 Hash deny 4 1 1 2
90 BEDaisy.sys\\9e2622d8e7a0ec136ba1fff639833f05137f8a1ff03e7a93b9a4aea25e7abb8d Hash deny 4 1 1 2
91 BEDaisy.sys\\a0e583bd88eb198558442f69a8bbfc96f4c5c297befea295138cfd2070f745c5 Hash deny 4 1 1 2
92 BEDaisy.sys\\a19fc837ca342d2db43ee8ad7290df48a1b8b85996c58a19ca3530101862a804 Hash deny 4 1 1 2
93 BEDaisy.sys\\af298d940b186f922464d2ef19ccfc129c77126a4f337ecf357b4fe5162a477c Hash deny 4 1 1 2
94 BEDaisy.sys\\b7bba82777c9912e6a728c3e873c5a8fd3546982e0d5fa88e64b3e2122f9bc3b Hash deny 4 1 1 2
95 BEDaisy.sys\\b9ed73af3aef69dc1fb91731d6d0a649e93f83d0f07ddb9729d71c2d00ed0801 Hash deny 4 1 1 2
96 BEDaisy.sys\\bceaf970b60b4457eca3c181f649a1c67f4602778171e53d9bdc9b97a09603ca Hash deny 4 1 1 2
97 BEDaisy.sys\\c0ae3349ebaac9a99c47ec55d5f7de00dc03bd7c5cd15799bc00646d642aa8de Hash deny 4 1 1 2
98 BEDaisy.sys\\c35f3a9da8e81e75642af20103240618b641d39724f9df438bf0f361122876b0 Hash deny 4 1 1 2
99 BEDaisy.sys\\c640930c29ea3610a3a5cebee573235ec70267ed223b79b9fa45a80081e686a4 Hash deny 4 1 1 2
100 BEDaisy.sys\\c8ff7c9f510f7a2ed88d9b336d8c9339698d5e1ee14bfb91aa89703ec06dce42 Hash deny 4 1 1 2
101 BEDaisy.sys\\cf66fcbcb8b2ea7fb4398f398b7480c50f6a451b51367718c36330182c1bb496 Hash deny 4 1 1 2
102 BEDaisy.sys\\d2e843d9729da9b19d6085edf69b90b057c890a74142f5202707057ee9c0b568 Hash deny 4 1 1 2
103 BEDaisy.sys\\dbebf6d463c2dbf61836b3eba09b643e1d79a02652a32482ca58894703b9addb Hash deny 4 1 1 2
104 BEDaisy.sys\\e89afd283d5789b8064d5487e04b97e2cd3fc0c711a8cec230543ebdf9ffc534 Hash deny 4 1 1 2
105 BEDaisy.sys\\eba14a2b4cefd74edaf38d963775352dc3618977e30261aab52be682a76b536f Hash deny 4 1 1 2
106 BEDaisy.sys\\edfc38f91b5e198f3bf80ef6dcaebb5e86963936bcd2e5280088ca90d6998b8c Hash deny 4 1 1 2
107 BEDaisy.sys\\f2ed6c1906663016123559d9f3407bc67f64e0d235fa6f10810a3fa7bb322967 Hash deny 4 1 1 2
108 BEDaisy.sys\\fa21e3d2bfb9fafddec0488852377fbb2dbdd6c066ca05bb5c4b6aa840fb7879 Hash deny 4 1 1 2
109 BEDaisy.sys\\ffd03584246730397e231eb8d16c1449aef2c3bc79bf9da3ebf8400a21b20ae7 Hash deny 4 1 1 2
110 Black.sys Hash deny 2 1 1 0
111 BlackBoneDrv10.sys Hash deny 2 1 1 0
112 BS_Flash64.sys Hash deny 4 1 1 2
113 BS_HWMIo64.sys Hash deny 4 1 1 2
114 BS_HWMIo64.sys\\6dafd15ee2fbce87fef1279312660fc399c4168f55b6e6d463bf680f1979adcf Hash deny 4 1 1 2
115 BS_RCIO\\1d0105b5e41fe0280f66d7a24eb00a04c03caaec Hash deny 1 1 0 0
116 BS_RCIO\\362c4f3dadc9c393682664a139d65d80e32caa2a97b6e0361dfd713a73267ecc Hash deny 4 1 1 2
117 BS_RCIO\\6191c20426dd9b131122fb97e45be64a4d6ce98cc583406f38473434636ddedc Hash deny 4 1 1 2
118 BS_RCIO\\73327429c505d8c5fd690a8ec019ed4fd5a726b607cabe71509111c7bfe9fc7e Hash deny 4 1 1 2
119 BS_RCIO\\d55b675941da4cc9be05f2ef7cea15784074772da585e5bf56d5be15afde4789 Hash deny 4 1 1 2
120 BS_RCIO\\d9111b2bedf78a769bb0799b964663cd119edaa8 Hash deny 1 1 0 0
121 BS_RCIO\\e9e711056fada8681f3eb5578a0d449b68568bc812e29dfcc0b92b9a9e481202 Hash deny 4 1 1 2
122 bw.sys Hash deny 2 1 1 0
123 bwrs.sys Hash deny 2 1 1 0
124 bwrsh.sys Hash deny 2 1 1 0
125 c.sys Hash deny 2 1 1 0
126 capcom.sys Hash deny 4 1 1 2
127 CodeSys SysDrv3S\\0dd9daf0852a5b1b436199e9f2bf318f641f43173ab0dc22ad8c7e9cbaee9ad3 Hash deny 4 1 1 2
128 CodeSys SysDrv3S\\161a50482380727ffa0dd94e193a023f4445dddd3a05340fe2db07fc3ec5b5f3 Hash deny 4 1 1 2
129 CodeSys SysDrv3S\\cf4efec43474c5aacf4b0971d44eaf8dd6357e594cdb1390085a5070a0df51d4 Hash deny 4 1 1 2
130 cpupress.sys Hash deny 2 1 1 0
131 d.sys Hash deny 2 1 1 0
132 d2.sys Hash deny 2 1 1 0
133 d3.sys Hash deny 4 2 2 0
134 d4.sys Hash deny 2 1 1 0
135 Datapath msr.sys\\6c6a4d07e95ab4212c2afefcb0ce37dc485fa56120b0419b636bd8bd326038c1.sys Hash deny 4 1 1 2
136 Datapath msr.sys\\ede9a3858a12d5ddea21a310e5721bf86c2248539f42c9e0c3c29ae5b0148ba5 Hash deny 4 1 1 2
137 DellBIOS.sys\\163912dfa4ad141e689e1625e994ab7c1f335410ebff0ade86bda3b7cdf6e065 Hash deny 4 1 1 2
138 DellBIOS.sys\\3678ba63d62efd3b706d1b661d631ded801485c08b5eb9a3ef38380c6cff319a Hash deny 4 1 1 2
139 DellBIOS.sys\\5bf3985644308662ebfa2fbcc11fb4d3e2a0c817ad3da1a791020f8c8589ebc8 Hash deny 4 1 1 2
140 DellBIOS.sys\\6575ea9b319beb3845d43ce2c70ea55f0414da2055fa82eec324c4cebdefe893 Hash deny 4 1 1 2
141 DellBIOS.sys\\700b9839fde53e91f0847053b4d2eb8d9bd3aca098844510f1fa3bab6a37eb24 Hash deny 4 1 1 2
142 DirectIO32\\035b96ff8b85d312be0f9df6271714392a802ec8bab59ae8229812ddc67ced5a Hash deny 4 1 1 2
143 DirectIO32\\0be4912bfd7a79f6ebfa1c06a59f0fb402bd4fe0158265780509edd0e562eac1 Hash deny 4 1 1 2
144 DirectIO32\\12d5a3d3f3226839c446bb5f7f2aa8dd7593d2bac8dd0d101d1c95145d10b01e Hash deny 4 1 1 2
145 DirectIO32\\16461fe1855e4cb4a5e3203f98a69376ad2dc8f69f1d43463206fdd6784b7fbf Hash deny 2 1 1 0
146 DirectIO32\\2fc5f41dd013af78fc594e427f462161f61bf72403b9795133cc89d28d962722 Hash deny 4 1 1 2
147 DirectIO32\\38b3eb8c86201d26353aab625cea672e60c2f66ce6f5e5eda673e8c3478bf305 Hash deny 4 1 1 2
148 DirectIO32\\3f9530c94b689f39cc83377d76979d443275012e022782a600dcb5cad4cca6aa Hash deny 4 1 1 2
149 DirectIO32\\65025741ecd0ef516da01319b42c2d96e13cb8d78de53fb7e39cd53ea6d58c75 Hash deny 4 1 1 2
150 DirectIO32\\72288d4978ee87ea6c8b1566dbd906107357087cef7364fb3dd1e1896d00baeb Hash deny 4 1 1 2
151 DirectIO32\\7dfc2eb033d2e090540860b8853036f40736d02bd22099ff6cf665a90be659cd Hash deny 4 1 1 2
152 DirectIO32\\e3b257357be41a18319332df7023c4407e2b93ac4c9e0c6754032e29f3763eac Hash deny 4 1 1 2
153 DirectIO32\\e4c154a0073bbad3c9f8ab7218e9b3be252ae705c20c568861dae4088f17ffcc Hash deny 4 1 1 2
154 DirectIO32\\fac102ef0a36d2d7b4390776a9c3edded72e01e7316949179e6fbe23495121fb Hash deny 4 1 1 2
155 EliteGroup ECSioDriver\\14edfdc13aeb98db50d597367f132443b086df0728f4fdb8c3bb5d47a8a0cd4a Hash deny 4 1 1 2
156 EnPortv.sys\\8af0621a8bb84196247f673bb76473406a3e87c0ca47813684a63b16b270cd30 Hash deny 4 1 1 2
157 EnPortv.sys\\df0966c93c5c3d47bca2805a5bfa00664c2f1bbaf0275f96fa9d8b2f21846f4b Hash deny 2 1 1 0
158 EnPortv.sys\\f831f0128c23e9b6559773e6d52890b511b1107aa730a4fed671379cb1c7008c Hash deny 2 1 1 0
159 fiddrv.sys Hash deny 4 1 1 2
160 fiddrv64.sys Hash deny 4 1 1 2
161 fidpcidrv.sys Hash deny 4 1 1 2
162 fidpcidrv64.sys Hash deny 4 1 1 2
163 Firewire kerneld\\02ed48cce245a294623123613053e3722b4af1c058524adfcad829ecb1253c45 Hash deny 2 1 1 0
164 Firewire kerneld\\065a34b786b0ccf6f88c136408943c3d2bd3da14357ee1e55e81e05d67a4c9bc Hash deny 2 1 1 0
165 Firewire kerneld\\0e99950bd89fdf47258fd40cfd6f47889491566f45f6820125a4d422ace726b7 Hash deny 2 1 1 0
166 Firewire kerneld\\125e4475a5437634cab529da9ea2ef0f4f65f89fb25a06349d731f283c27d9fe Hash deny 2 1 1 0
167 Firewire kerneld\\1336469ec0711736e742b730d356af23f8139da6038979cfe4de282de1365d3b Hash deny 2 1 1 0
168 Firewire kerneld\\18047c2d45758a43d6b7e56bcd4aa90354c899795baf944f037850c48d8e892a Hash deny 2 1 1 0
169 Firewire kerneld\\1891d48fb8829e199552d7e022c2a4607e03b29101ef969ef508db5223898a78 Hash deny 2 1 1 0
170 Firewire kerneld\\212c05b487cd4e64de2a1077b789e47e9ac3361efa24d9aab3cc6ad4bd3bd76a Hash deny 2 1 1 0
171 Firewire kerneld\\230b55436d02580adc14e8eb5602b6d4c033d18df21dece11dd22acf8f64c432 Hash deny 2 1 1 0
172 Firewire kerneld\\263e2adc9b96de26d8d5c482659dc3927a2ccb90e7e71f4c726c2527d64979d6 Hash deny 2 1 1 0
173 Firewire kerneld\\26c5e61293fb9ac935862bb38e3368aa5db3a5a5287d344d16e942c03e0726a0 Hash deny 2 1 1 0
174 Firewire kerneld\\26ff8947acdb9dbfd1dbbf008b2963c6adcbe475333f673d760aa34fba8fe324 Hash deny 2 1 1 0
175 Firewire kerneld\\27375351b4a723465f937866f0ffc86e8e612b093673ee25ccf0b7ea803f888f Hash deny 2 1 1 0
176 Firewire kerneld\\2f4c2f50e8e742751e1373550f782866cb4698392e4fb7b25c20cef3a6385ea8 Hash deny 2 1 1 0
177 Firewire kerneld\\3277e4bfee77544ee2484b1eb5d9513227da5fe0bbd03491be589f9a5ac9781e Hash deny 2 1 1 0
178 Firewire kerneld\\33bc9a17a0909e32a3ae7e6f089b7f050591dd6f3f7a8172575606bec01889ef Hash deny 2 1 1 0
179 Firewire kerneld\\442f12adebf7cb166b19e8aead2b0440450fd1f33f5db384a39776bb2656474a Hash deny 2 1 1 0
180 Firewire kerneld\\486321368b19931fc96517e59626763dee321fc539a416917030cf481a715c13 Hash deny 2 1 1 0
181 Firewire kerneld\\51f002ee44e46889cf5b99a724dd10cc2bd3e22545e2a2cb3bd6b1dd3af5ba11 Hash deny 2 1 1 0
182 Firewire kerneld\\53b9e423baf946983d03ce309ec5e006ba18c9956dcd97c68a8b714d18c8ffcf Hash deny 2 1 1 0
183 Firewire kerneld\\582b62ffbcbcdd62c0fc624cdf106545af71078f1edfe1129401d64f3eefaa3a Hash deny 2 1 1 0
184 Firewire kerneld\\6297556f66cd6619057f3a5b216b314f8a27eebb5fa575ee07a1944aca71ae80 Hash deny 2 1 1 0
185 Firewire kerneld\\680ddece32fe99f056e770cb08641f5b585550798dfdf723441a11364637c7e6 Hash deny 2 1 1 0
186 Firewire kerneld\\6ef0b34649186fb98a7431b606e77ee35e755894b038755ba98e577bd51b2c72 Hash deny 2 1 1 0
187 Firewire kerneld\\748ccadb6bf6cdf4c5a5a1bb9950ee167d8b27c5817da71d38e2bc922ffce73d Hash deny 2 1 1 0
188 Firewire kerneld\\7635cab17e17cc1004c9c4ad7cbaddbf94e8aaca3f820c837fb2af54254b3874 Hash deny 2 1 1 0
189 Firewire kerneld\\76940e313c27c7ff692051fbf1fbdec19c8c31a6723a9de7e15c3c1bec8186f6 Hash deny 2 1 1 0
190 Firewire kerneld\\789a18f7a16616c8bee092e271b70f9a8f5c2c62fe547f62044fc33a2c934074 Hash deny 2 1 1 0
191 Firewire kerneld\\895aeabfb01d068a2ff648d6d0eb608dcfda654c5bc2e83aa83293bc9ee84680 Hash deny 2 1 1 0
192 Firewire kerneld\\8edab185e765f9806fa57153db1ede00e68270d2351443ee1de30674eca8d9b6 Hash deny 2 1 1 0
193 Firewire kerneld\\8f995a9fab401dbb5e474c4feffb00b8ae147d69de387d5b0daf5e3927e48be5 Hash deny 2 1 1 0
194 Firewire kerneld\\9aad1d3403ebe140a4eda10f70b63973114a14e9c329e79b79b574628150f1b7 Hash deny 2 1 1 0
195 Firewire kerneld\\a188760f1bf36584a2720014ca982252c6bcd824e7619a98580e28be6090dccc Hash deny 2 1 1 0
196 Firewire kerneld\\b1e4455499c6a90ba9a861120a015a6b6f17e64479462b869ad0f05edf6552de Hash deny 2 1 1 0
197 Firewire kerneld\\bac7e75745d0cb8819de738b73edded02a07111587c4531383dccd4562922b65 Hash deny 2 1 1 0
198 Firewire kerneld\\bd3cf8b9af255b5d4735782d3653be38578ff5be18846b13d05867a6159aaa53 Hash deny 2 1 1 0
199 Firewire kerneld\\cb59a641adb623a65a9b5af1db2ffd921fd1ca1bc046a6df85d5f2e00fd0b5a5 Hash deny 2 1 1 0
200 Firewire kerneld\\d330ab003206ce5e9828607562790aa8dd0453f6b7452f5c6053e3c6b6761d25 Hash deny 2 1 1 0
201 Firewire kerneld\\d3b5fd13a53eee5c468c8bfde4bfa7b968c761f9b781bb80ccd5637ee052ee7d Hash deny 2 1 1 0
202 Firewire kerneld\\db0d425708ba908aedf5f8762d6fdca7636ae3a537372889446176c0237a2836 Hash deny 2 1 1 0
203 Firewire kerneld\\dfe57c6a4ef4d2491be325d67428698a61d9c5d2a24dbada10043d313be2c8cc Hash deny 2 1 1 0
204 Firewire kerneld\\e9919d1546c7dfef62ff01b87f739812de0a57463611c12012013ae689023ce1 Hash deny 2 1 1 0
205 full.sys Hash deny 2 1 1 0
206 gameink.sys Hash deny 8 4 4 0
207 GameTerSafe.sys Hash deny 2 1 1 0
208 GEDevDrv.sys\\51145a3fa8258aac106f65f34159d23c54b48b6d54ec0421748b3939ab6778eb Hash deny 4 1 1 2
209 GEDevDrv.sys\\a369942ce8d4b70ebf664981e12c736ec980dbe5a74585dd826553c4723b1bce Hash deny 4 1 1 2
210 GEDevDrv.sys\\ae73dd357e5950face9c956570088f334d18464cd49f00c56420e3d6ff47e8dc Hash deny 4 1 1 2
211 GEDevDrv.sys\\cac5dc7c3da69b682097144f12a816530091d4708ca432a7ce39f6abe6616461 Hash deny 4 1 1 2
212 Gigabyte gvcidrv\\42f0b036687cbd7717c9efed6991c00d4e3e7b032dc965a2556c02177dfdad0f Hash deny 4 1 1 2
213 GLCKIO2.sys Hash deny 4 1 1 2
214 gmer\\4d922fcaf6f6c794218fd2db946a650fe96a6476ebeade84372b0c4defc730d6 Hash deny 2 1 1 0
215 gmer\\611223d444872a2ccea59cfa775e29fa0b84e9dc24c24cffe1316691d48d9593 Hash deny 2 1 1 0
216 gmer\\95707b3d009cd749a4352e59e4f1b1a4b291a02066bd45c3c338fa7591749914 Hash deny 2 1 1 0
217 GVCIDrv64.sys Hash deny 4 1 1 2
218 Hangzhou RentDrv.sys\\1aed62a63b4802e599bbd33162319129501d603cceeb5e1eb22fd4733b3018a3 Hash deny 4 1 1 2
219 Hangzhou RentDrv.sys\\39c128ceabfb170ef5fdbe7f7740de48a9d9119df2e01763c25995cea1097785 Hash deny 4 1 1 2
220 Hangzhou RentDrv.sys\\9165d4f3036919a96b86d24b64d75d692802c7513f2b3054b20be40c212240a5 Hash deny 4 1 1 2
221 hlpdrv\\24021015981\\63da1ad735c606dea5b33d892420c38448773cd574d437c1c7efcccdae0266e4 Hash deny 2 1 1 0
222 hlpdrv\\24021015981\\bd1f381e5a3db22e88776b7873d4d2835e9a1ec620571d2b1da0c58f81c84a56 Hash deny 4 1 1 2
223 HP SSPORT.sys Hash deny 1 0 0 0
224 HP SSPORT.sys\\09e863170e546b5889ad02f1effecf6ec8ea0a99d02878548c0415e460618c88 Hash deny 1 0 0 0
225 HP SSPORT.sys\\725d4445c65e1bf94c9fc8f07961512a8ad22628515bfa789b321a3169e0b65a Hash deny 1 0 0 0
226 HP SSPORT.sys\\83a3159aded44712ae5413743631abe387192edf84f33cdae623c5d94f2ffb01 Hash deny 1 0 0 0
227 HP SSPORT.sys\\b3ea41eabb11e18413e48fe1f7fef37635a464848385f90f7a28498d144bee46 Hash deny 1 0 0 0
228 HP SSPORT.sys\\b753e00cffcca9127f40a837cacda8cee4085d33548380fc73a9c5cddc20489a Hash deny 1 0 0 0
229 HP SSPORT.sys\\b7ddfe870f76851a7dee6687d1ef48fb70e52b400ffe950021d58c2ad9e51959 Hash deny 1 0 0 0
230 hw_sys\\b8fcc8ef2b27c0c0622d069981e39f112d3b3b0dbede053340bc157ba1316eab Hash deny 4 1 1 2
231 HwRwDrv.sys\\42de79eb237293befb1b954beaf92b832f947195e3c359048aaa464ead56b62d Hash deny 2 1 1 0
232 HwRwDrv.sys\\d50ee14181cf60bbdffe1a891b9bb3a852c93019f1f05dde47b3178b821b8f54 Hash deny 2 1 1 0
233 HwRwDrv.sys\\f2b95fc91fe33c1995c49c35e32124ece7d958ed7d3b7a5f325f2a30454b9256 Hash deny 2 1 1 0
234 HyperTech wfshbr64\\89698cad598a56f9e45efffd15d1841e494a2409cc12279150a03842cd6bb7f3 Hash deny 4 1 1 2
235 HyperTech wfshbr64\\b8807e365be2813b7eccd2e4c49afb0d1e131086715638b7a6307cd7d7e9556c Hash deny 4 1 1 2
236 IBM sysconp.sys\\dba8db472e51edd59f0bbaf4e09df71613d4dd26fd05f14a9bc7e3fc217a78aa Hash deny 4 1 1 2
237 IBM sysconp.sys\\df4c02beb039d15ff0c691bbc3595c9edfc1d24e783c8538a859bc5ea537188d Hash deny 4 1 1 2
238 IDTech MSRHook\\10397e0ebd622b20412fcfcd28f832ee562181931192a58cc19dfd45a98e684f Hash deny 4 1 1 2
239 IDTech MSRHook\\4e4dcb68457b4615e38e8c3148d4303e98c166e56b4aa9fae2dee8d24d3e93e9 Hash deny 4 1 1 2
240 IDTech MSRHook\\6a374d023813382fb79b447c05f3382f9d0bbb13f8ab0c1f8e8168f4a23d5ffe Hash deny 4 1 1 2
241 IDTech MSRHook\\79c14ffa8e8d28b78a9a2f0618052a64e83c98acc4aeec27178c0002f9c91dae Hash deny 4 1 1 2
242 IDTech MSRHook\\a0ba1c981dcf3930680c97664efce6142679bd84604c38eb8b8368f6c1bde3c9 Hash deny 4 1 1 2
243 IDTech MSRHook\\d0b65b4277ab975c7e0579839153dfb8febeaf8ca3aa621bc710cd7bcaaa8ad1 Hash deny 4 1 1 2
244 IDTech MSRHook\\f4b447891887806b4712986f00e10612f8d9dbda1cb1ec1de032fb4c34199428 Hash deny 4 1 1 2
245 IDTech MSRHook\\fc6d327b8af44b6adeffc4c3c00ba9edda41d40dae3ff2e1449125e3eb08e15c Hash deny 4 1 1 2
246 iFlyWinRing0x64.sys\\09d8bc4e499e895fe55648381ce1c46879b45c1f1e3db3f4ed5937475372bc58 Hash deny 1 0 0 0
247 iFlyWinRing0x64.sys\\e1d11927370965dbd769f9270876a3b6839631d9b523c7a26d9de7761279f008 Hash deny 1 0 0 0
248 inpoutx\\2d83ccb1ad9839c9f5b3f10b1f856177df1594c66cbbc7661677d4b462ebf44d Hash deny 4 1 1 2
249 inpoutx\\7aed38beff4d59d57b43a32738a1a30a7e0eba6a Hash deny 1 1 0 0
250 inpoutx\\945ee05244316ff2f877718cf0625d4eb34e6ec472f403f958f2a700f9092507 Hash deny 4 1 1 2
251 inpoutx\\a88733b88cdc3f3cc040912ce5a3c44fa26f2ea8454cf6fc855b104a4910fa31 Hash deny 2 1 1 0
252 inpoutx\\b8ded5e10dfc997482ba4377c60e7902e6f755674be51b0e181ae465529fb2f2 Hash deny 2 1 1 0
253 inpoutx\\cfab93885e5129a86d13fd380d010cc8c204429973b776ab1b472d84a767930f Hash deny 2 1 1 0
254 inpoutx\\d5cc046c2ae9ba6fe54def699f1c4fa92d3226304321bbf45cc33883ce131138 Hash deny 2 1 1 0
255 inpoutx\\e2c531a771b0df1585518a22427798e86611e6be3d357024797871a1b3876e9c Hash deny 2 1 1 0
256 Inspect EchoDriver\\a41e9bb037cf1dc2237659b1158f0ed4e49b752b2f9dae4cc310933a9d1f1e47 Hash deny 4 1 1 2
257 Inspect EchoDriver\\ada2b855757c9062231f5ed4e80365b8d8094e9adbce8f26d1ff5ea0b7a70c77 Hash deny 4 1 1 2
258 Inspect EchoDriver\\ea3c5569405ed02ec24298534a983bcb5de113c18bc3fd01a4dd0b5839cd17b9 Hash deny 4 1 1 2
259 IoAccess.sys\\0191f9a1d0f7b3a063ee538074181aee92399472562623b49b75070614cd535e Hash deny 1 0 0 0
260 IoAccess.sys\\0b135632111316facbbf06fe8ecf1522cb415745875a329abe070881bfe2a555 Hash deny 1 0 0 0
261 IoAccess.sys\\0b98023ef571fb162133e79c776b0840a3bdbd55b56752a791ed05a769caa495 Hash deny 1 0 0 0
262 IoAccess.sys\\1215acbdd8720fdcf5bafd36c94fc41e25cd367617c5b7bcaca0a30eeb328981 Hash deny 1 0 0 0
263 IoAccess.sys\\1d1f21f81c479c8ce333801e39e8064fff40d466d42ebd56bb1f4861fc86190a Hash deny 1 0 0 0
264 IoAccess.sys\\208b4361be8e5011423f210b415b4d9b397f1893 Hash deny 1 0 0 0
265 IoAccess.sys\\2f04f0f73e766446ea8f7b8d9d3fb78affea7fb4f16e18747aeed65c6e9958e8 Hash deny 1 0 0 0
266 IOAccess.sys\\4044d50fbd7c25313b74f4b3a29e8db153c2974ba2a13b7282b20074a40021f0 Hash deny 1 0 0 0
267 IoAccess.sys\\5b2ea11f9404726ffc750f4a175d69c41a3253da9d9217caece2477288c05ad6 Hash deny 1 0 0 0
268 IoAccess.sys\\5e31f40073017beffbc53bdc887c2f99b168bb64ba432b9b5ba821b971d2734f Hash deny 1 0 0 0
269 IoAccess.sys\\6a92170c71bf052922aebd1136842ccbf5942e6b53740d9cfd9de6c58bc13b58 Hash deny 2 0 0 0
270 IoAccess.sys\\73a7cb8a00b70e4b289a147490f0a5ed4805081f88ffd8cf6ce8fc3b96c004d4 Hash deny 1 0 0 0
271 IoAccess.sys\\78a7f5d1868a216de6451218c1c7d6f8cc6b870ca683f89a7d08d51615a84c4f Hash deny 1 0 0 0
272 IoAccess.sys\\b03759640798b7c3b8079d0cb662fce9ad8a2e25f9acf3fda29d5ec53b5cd2bc Hash deny 1 0 0 0
273 IoAccess.sys\\b9e0c2a569ab02742fa3a37846310a1d4e46ba2bfd4f80e16f00865fc62690cb Hash deny 1 0 0 0
274 IoAccess.sys\\bd724ef0c3bf9f160df2c5b15f25048f36be869218f40e642f326e0ef4097394 Hash deny 1 0 0 0
275 IoAccess.sys\\be227767ed87b594f581eafdf7a938eb3607c5f389ad73f8924635b825e9681e Hash deny 1 0 0 0
276 IoAccess.sys\\c3222b982909d25f699a0a204c3b045694eb6b811b9c1f4a70b2d856fb11a0a9 Hash deny 1 0 0 0
277 IoAccess.sys\\d616c0cbce2576e29192d6bf8b37694fc8014b134a884b55afd8793990569d0f Hash deny 1 0 0 0
278 IoAccess.sys\\ee7e65fa2491e0033fa13030eb0213fbebc1e834c18fa2cff0fa33d198f9e797 Hash deny 1 0 0 0
279 IoAccess.sys\\fb99295b97a4906d5df8fef3d42305d5765cd37c88722f3b1dc84b2af285f38d Hash deny 1 0 0 0
280 IoBitUnlocker\\0209934453e9ce60b1a5e4b85412e6faf29127987505bfb1185fc9296c578b09 Hash deny 2 1 1 0
281 IoBitUnlocker\\0aff83f28d70f425539fee3d6a780210d0406264f8a4eb124e32b074e8ffd556 Hash deny 4 1 1 2
282 IoBitUnlocker\\103d7e0387358f7b44a2e4c2da483fe0e854b720b6544f914caeb5be9dccfb93 Hash deny 2 1 1 0
283 IoBitUnlocker\\11bc55c0771d692279298211c1d434c04168e7c7f7c4328bfd600215b88c819b Hash deny 4 1 1 2
284 IoBitUnlocker\\29cf2d374d7afe009bbf60ba5f50db7016314de682cf3a6f90c0996810c821ef Hash deny 2 1 1 0
285 IoBitUnlocker\\4e1d75684923974c0333d33b789c5d1569ba5a39e8fa6816e825eadaeaf51a2a Hash deny 2 1 1 0
286 IoBitUnlocker\\5ea5f339b2e40dea57378626790ca7e9a82777aacdada5bc61ebb7d82043fa07 Hash deny 4 1 1 2
287 IoBitUnlocker\\698353791261d5a9ca3245ae8f86334493df554690ec7962895c2affe4050db2 Hash deny 4 1 1 2
288 IoBitUnlocker\\7c12d9151c57fef87d6e1c89f88bd1602bf64d215533ad3cd627ddefbd220075 Hash deny 2 1 1 0
289 IoBitUnlocker\\831b62145c21557928a694e6261e830f1545b5756ad51dcbd28a15fde570f4e7 Hash deny 4 1 1 2
290 IoBitUnlocker\\88d2a2262e7789180dfefeab0751d28814b87eb9d3b4bc201cc1d18cbc35e0cf Hash deny 4 1 1 2
291 IoBitUnlocker\\969f73a1da331e43777a3c1f08ec0734e7cf8c8136e5d469cbad8035fbfe3b47 Hash deny 4 1 1 2
292 IoBitUnlocker\\a7416a7d9573f1d8873ec1b3109ec683e85412ba817e0001c3ab2d2c92043d4d Hash deny 4 1 1 2
293 IoBitUnlocker\\c2e1a3dd0dfb3477a3e855368b23d12b8818df8fa3bc3508abf069a0873d6bf8 Hash deny 4 1 1 2
294 IoBitUnlocker\\e41d4fd99252fcf9aea529b6e148b311aa26a4ab04f6b79cce4cd19c61db0c87 Hash deny 4 1 1 2
295 IREC.sys\\irec32--1.sys Hash deny 4 1 1 2
296 IREC.sys\\irec32--10.sys Hash deny 4 1 1 2
297 IREC.sys\\irec32--11.sys Hash deny 4 1 1 2
298 IREC.sys\\irec32--2.sys Hash deny 4 1 1 2
299 IREC.sys\\irec32--3.sys Hash deny 4 1 1 2
300 IREC.sys\\irec32--5.sys Hash deny 4 1 1 2
301 IREC.sys\\irec32--7.sys Hash deny 4 1 1 2
302 IREC.sys\\irec32--8.sys Hash deny 4 1 1 2
303 IREC.sys\\irec32--9.sys Hash deny 4 1 1 2
304 IREC.sys\\irec64--16.sys Hash deny 4 1 1 2
305 IREC.sys\\irec64--17.sys Hash deny 4 1 1 2
306 IREC.sys\\irec64--18.sys Hash deny 4 1 1 2
307 IREC.sys\\irec64--19.sys Hash deny 4 1 1 2
308 IREC.sys\\irec64--21.sys Hash deny 4 1 1 2
309 IREC.sys\\irec64--23.sys Hash deny 4 1 1 2
310 IREC.sys\\irec64--24.sys Hash deny 4 1 1 2
311 IREC.sys\\irec64--25.sys Hash deny 4 1 1 2
312 IREC.sys\\irec64--26.sys Hash deny 4 1 1 2
313 IREC.sys\\irec64--27.sys Hash deny 4 1 1 2
314 IREC.sys\\irec64--28.sys Hash deny 4 1 1 2
315 IREC.sys\\irecARM64--45.sys Hash deny 4 1 1 2
316 ITM probmon.sys\\023d722cbbdd04e3db77de7e6e3cfeabcef21ba5b2f04c3f3a33691801dd45eb Hash deny 2 1 1 0
317 ITM probmon.sys\\c2026232d39f5b0a8e9f15da8cb8f74e550b9498ae3b4015fb17fcc5d580d98b Hash deny 2 1 1 0
318 ITM s4killer\\1ae892a9d98d5bbe1cacc1b5aaf224c333db985ddb621d75421df647a0765a4f Hash deny 2 1 1 0
319 ITM s4killer\\8ed196199d309818f5eec25b083661c539b62cd4dc0f86b44561ea30acc65914 Hash deny 2 1 1 0
320 ITM s4killer\\91daa81e50b365589629afa1ac05117db111f659ccc344d3314844048ce6060b Hash deny 2 1 1 0
321 ITM s4killer\\cf1a78df830218f8e675ffd1467b53534a0b514cda9aef0f227ce210c93e1651 Hash deny 4 1 1 2
322 ITM s4killer\\deeedd90afd35fe4bd5ff919ab860b60f8c5d6145a8fbd1c20ee0df1c8cf4543 Hash deny 2 1 1 0
323 kbdcap64.sys Hash deny 2 1 1 0
324 kerncorelib.sys\\7196cc5f4259d53f0badbc56d4d27ec39e13a622ae4dd34d99a0b2248a6d653b Hash deny 2 1 1 0
325 lgcoretemp\\e0cb07a0624ddfacaa882af49e3783ae02c9fbd0ab232541a05a95b4a8abd8ef Hash deny 4 1 1 2
326 LgDCatcher.sys Hash deny 8 4 4 0
327 Lurker.sys Hash deny 2 1 1 0
328 mhyprot2.sys\\247AADAF17ED894FCACF3FC4E109B005540E3659FD0249190EB33725D3D3082F Hash deny 4 1 1 2
329 mhyprot2.sys\\26D69E677D30BB53C7AC7F3FCE76291FE2C44720EF17EE386F95F08EC5175288 Hash deny 4 1 1 2
330 mhyprot2.sys\\46CF46E1073B7C99142964B7C4BEF1E5285FABCF2C6DBE5BE99000A393D9F474 Hash deny 4 1 1 2
331 mhyprot2.sys\\509628B6D16D2428031311D7BD2ADD8D5F5160E9ECC0CD909F1E82BBBB3234D6 Hash deny 4 1 1 2
332 mhyprot2.sys\\6E76764D750EBD835AA4BB055830D278DF530303585614C1DC743F8D5ADF97D7 Hash deny 4 1 1 2
333 mhyprot2.sys\\AD2477632B9B07588CFE0E692F244C05FA4202975C1FE91DD3B90FA911AC6058 Hash deny 4 1 1 2
334 mhyprot2.sys\\B8B94C2646B62F6AC08F16514B6EFAA9866AA3C581E4C0435A7AEAFE569B2418 Hash deny 4 1 1 2
335 mhyprot3.sys\\475E5016C9C0F5A127896F9179A1B1577A67B357F399AB5A1E68AAB07134729A Hash deny 4 1 1 2
336 mhyprot3.sys\\7FD90500B57F9AC959C87F713FE9CA59E669E6E1512F77FCCB6A75CDC0DFEE8E Hash deny 4 1 1 2
337 mhyprot3.sys\\8F3323053381B922681D26D9BA53A01D63B07D53BFCD36AE87B295BBCEC27F65 Hash deny 2 1 1 0
338 mhyprot3.sys\\B531F0A11CA481D5125C93C977325E135A04058019F939169CE3CDEDADDD422D Hash deny 4 1 1 2
339 mhyprot3.sys\\B617A072C578CEA38C460E2851F3D122BA1B7CFA1F5EE3E9F5927663AC37AF61 Hash deny 4 1 1 2
340 mhyprotect.sys\\8bdcf7457c2caf7fa0386571f972d7f5220d385ad686e2c3536f4c67ba4333e6 Hash deny 4 1 1 2
341 mhyprotect.sys\\edeb35e4341034b2de389017c4884b081a821f34349a620897a2a845c84cb09e Hash deny 4 1 1 2
342 mhyprotnap.sys\\40263b08b3c3659529ab605d1daa3033db0fdc4b19c26aa375be0c19686807e6 Hash deny 4 1 1 2
343 mhyprotrpg.sys\\8bf84bed9b5fa4576182c84d2f31679dc472acd0f83c9813498e9f71ed9fef3e Hash deny 4 1 1 2
344 mhyprotrpg.sys\\f7d72d22cd4ad3e44fd617bdb4c90b9a884f4eb045688c0e3fb64dd33e033eaa Hash deny 4 1 1 2
345 MsIo.sys Hash deny 8 3 3 2
346 MsIo.sys\\525d9b51a80ca0cd4c5889a96f857e73f3a80da1ffbae59851e0f51bdfb0b6cd Hash deny 4 1 1 2
347 My.sys Hash deny 2 1 1 0
348 netfilterdrv.sys Hash deny 22 11 11 0
349 NetFlt.sys Hash deny 2 1 1 0
350 NetProxyDriver.sys Hash deny 2 1 1 0
351 ni.sys Hash deny 2 1 1 0
352 Niche Technologies pplkiller.sys\\0d176493a5be4c94744836da1c365707e49f40c237c29c3481ae5677f5fdbba1 Hash deny 2 1 1 0
353 Noriyuki Miyazaki SysInfo\\4fea15aabc4fc63a3e991412caf17283bbd257172ef7e255f40f5e22e0286902 Hash deny 2 1 1 0
354 Noriyuki Miyazaki SysInfo\\7049f3c939efe76a5556c2a2c04386db51daf61d56b679f4868bb0983c996ebb Hash deny 4 1 1 2
355 Noriyuki Miyazaki SysInfo\\b85e9b69ad23bfb37452fe0b67dfa71e5980a8e4310b021bc6f8c36f893bc625 Hash deny 2 1 1 0
356 nstr.sys Hash deny 2 1 1 0
357 nstrwsk.sys Hash deny 2 1 1 0
358 nt2.sys Hash deny 2 1 1 0
359 nt3.sys Hash deny 2 1 1 0
360 nt4.sys Hash deny 4 2 2 0
361 nt5.sys Hash deny 2 1 1 0
362 nt6.sys Hash deny 2 1 1 0
363 nvflash.sys\\0a89a6ab2fca486480b6e3dacf392d6ce0c59a5bdb4bcd18d672feb4ebb0543c Hash deny 4 1 1 2
364 nvflash.sys\\0e9072759433abf3304667b332354e0c635964ff930de034294bf13d40da2a6f Hash deny 4 1 1 2
365 nvflash.sys\\159dcf37dc723d6db2bad46ed6a1b0e31d72390ec298a5413c7be318aef4a241 Hash deny 2 1 1 0
366 nvflash.sys\\1ce9e4600859293c59d884ea721e9b20b2410f6ef80699f8a78a6b9fad505dfc Hash deny 4 1 1 2
367 nvflash.sys\\1e9ec6b3e83055ae90f3664a083c46885c506d33de5e2a49f5f1189e89fa9f0a Hash deny 4 1 1 2
368 nvflash.sys\\20dd9542d30174585f2623642c7fbbda84e2347e4365e804e3f3d81f530c4ece Hash deny 4 1 1 2
369 nvflash.sys\\3b2ad08123e8ed2516548240cfcdf5eefd89293f31070a6cd3949ee1b66fed14 Hash deny 4 1 1 2
370 nvflash.sys\\4115b7a30061d11a034188c0ec7a2223f3b032c8b3420cfffabf6c4df692920d Hash deny 2 1 1 0
371 nvflash.sys\\423d58265b22504f512a84faf787c1af17c44445ae68f7adcaa68b6f970e7bd5 Hash deny 4 1 1 2
372 nvflash.sys\\4710acca9c4a61e2fc6daafb09d72e11b603ef8cd732e12a84274ea9ad6d43be Hash deny 2 1 1 0
373 nvflash.sys\\4737750788c72d2fc9cf95681c622357263075d65b23e54c4dc3f31446cad37b Hash deny 4 1 1 2
374 nvflash.sys\\50aa2b3a762abb1306fa003c60de3c78e89ea5d29aab8a9c6479792d2be3c2d7 Hash deny 4 1 1 2
375 nvflash.sys\\5cc6b647174c8efa0a81ec1d3cb0464c8a567456571d0939fb2e76c6850bf7cb Hash deny 4 1 1 2
376 nvflash.sys\\747a4dc50915053649c499a508853a42d9e325a5eec22e586571e338c6d32465 Hash deny 4 1 1 2
377 nvflash.sys\\79aa2cedd1b8415ba6d00f4b3601e2363c8bdd07f860a3b8de010f9e5187c0e9 Hash deny 2 1 1 0
378 nvflash.sys\\7c933f5d07ccb4bd715666cd6eb35a774b266ddd8d212849535a54192a44f667 Hash deny 4 1 1 2
379 nvflash.sys\\7ef8949637cb947f1a4e1d4e68d31d1385a600d1b1054b53e7417767461fafa7 Hash deny 4 1 1 2
380 nvflash.sys\\8162811e8aae05884e8cb84b8dd87c310e5ed5ec588b9023a4d849d558d6ae34 Hash deny 2 1 1 0
381 nvflash.sys\\835733590a778f48dae1df4e33da8455b89449fed3e04fa19b64bbdcb6a530db Hash deny 2 1 1 0
382 nvflash.sys\\9155470dc24449977d1be15a116b08705dd4c113a2eb4ab19a6000749ff4b100 Hash deny 2 1 1 0
383 nvflash.sys\\9368e51ec98e2ad20893a5fc21e6a8b20c5bee158d5c49ca58649cff84db9d68 Hash deny 2 1 1 0
384 nvflash.sys\\b715d5682ab59a0ce3f858e47bf79bdf876a899f618c12c22b27cb1dd4daa8f4 Hash deny 2 1 1 0
385 nvflash.sys\\c11305fc8da85568b2d41cdf030ce260815fea848af91dc0e01076d461bab919 Hash deny 2 1 1 0
386 nvflash.sys\\c188b36f258f38193ace21a7d254f0aec36b59ad7e3f9bcb9c2958108effebad Hash deny 4 1 1 2
387 nvflash.sys\\df996d5a06a2e2ecc087569358b1957d500b176ec7ed37031bcee440963d9d80 Hash deny 2 1 1 0
388 nvflash.sys\\e2d6cdc3d8960a50d9f292bb337b3235956a61e4e8b16cf158cb979b777f42aa Hash deny 4 1 1 2
389 nvflash.sys\\eef68fdc5df91660410fb9bed005ed08c258c44d66349192faf5bb5f09f5fa90 Hash deny 4 1 1 2
390 nvflash.sys\\f1fbec90c60ee4daba1b35932db9f3556633b2777b1039163841a91cf997938e Hash deny 4 1 1 2
391 nvflash.sys\\f2a4ddc38e68efd2eac27b2562529926f5ade93575a82e8d3e0abb2b37347257 Hash deny 4 1 1 2
392 nvflash.sys\\f583cfb8aab7d084dc052dbd0b9d56693308cbb26bd1b607c2aedf8ee2b25e44 Hash deny 4 1 1 2
393 nvflash.sys\\fd94be9ac97f06abe64426933fbee02871d5d181b1d9025daf1aaa92d9342e90 Hash deny 2 1 1 0
394 nvflash.sys\\fe2fb5d6cfcd64aeb62e6bf5b71fd2b2a87886eb97ab59e5353ba740da9f5db5 Hash deny 4 1 1 2
395 nvflash.sys\\ffd1aef19646ffed09b56a2ace4fc8cdf5b2f714fcca1e7ffb82256264c94b18 Hash deny 4 1 1 2
396 nvoclock\\2203bd4731a8fdc2a1c60e975fd79fd5985369e98a117df7ee43c528d3c85958 Hash deny 4 1 1 2
397 nvoclock\\29f449fca0a41deccef5b0dccd22af18259222f69ed6389beafe8d5168c59e36 Hash deny 2 1 1 0
398 nvoclock\\3cb111fdedc32f2f253aacde4372b710035c8652eb3586553652477a521c9284 Hash deny 2 1 1 0
399 nvoclock\\4d777a9e2c61e8b55b3c34c5265b301454bb080abe7ffb373e7800bd6a498f8d Hash deny 2 1 1 0
400 nvoclock\\64a8e00570c68574b091ebdd5734b87f544fa59b75a4377966c661d0475d69a5 Hash deny 4 1 1 2
401 nvoclock\\77da3e8c5d70978b287d433ae1e1236c895b530a8e1475a9a190cdcc06711d2f Hash deny 2 1 1 0
402 nvoclock\\87b4c5b7f653b47c9c3bed833f4d65648db22481e9fc54aa4a8c6549fa31712b Hash deny 2 1 1 0
403 nvoclock\\d7c90cf3fdbbd2f40fe6a39ad0bb2a9a97a0416354ea84db3aeff6d925d14df8 Hash deny 2 1 1 0
404 Omron FH-Ether\\ae71f40f06edda422efcd16f3a48f5b795b34dd6d9bb19c9c8f2e083f0850eb7 Hash deny 2 1 1 0
405 otipcibus.sys\\4e3eb5b9bce2fd9f6878ae36288211f0997f6149aa8c290ed91228ba4cdfae80 Hash deny 4 1 1 2
406 PartnerTech WinIO32A.sys\\01779ee53f999464465ed690d823d160f73f10e7 Hash deny 1 1 0 0
407 PartnerTech WinIO32B.sys\\f1c8c3926d0370459a1b7f0cf3d17b22ff9d0c7f Hash deny 1 1 0 0
408 PartnerTech WinIo64A.sys\\0c74d09da7baf7c05360346e4c3512d0cd433d59 Hash deny 1 1 0 0
409 PartnerTech WinIo64B.sys\\f18e669127c041431cde8f2d03b15cfc20696056 Hash deny 1 1 0 0
410 PartnerTech WinIO64C.sys\\a65fabaf64aa1934314aae23f25cdf215cbaa4b6 Hash deny 1 1 0 0
411 PartnerTech WinIo64C.sys\\b242b0332b9c9e8e17ec27ef10d75503d20d97b6 Hash deny 1 1 0 0
412 PartnerTech WinIO\\3243aab18e273a9b9c4280a57aecef278e10bfff19abb260d7a7820e41739099 Hash deny 4 1 1 2
413 PartnerTech WinIO\\3c9b6da610e409f92f4f95f6f3f92a6e60e24903298a0e9af508f28e8c8962b6 Hash deny 4 1 1 2
414 PartnerTech WinIO\\51e280cd9d1d84d43fab4a7be894804f24a1ca4d39f1df16fd8c60ea0a43b786 Hash deny 4 1 1 2
415 PartnerTech WinIO\\752565bab29cd2c63b4ff59a8c637bed02c2689781067ddf7cfc5c5221eb1d68 Hash deny 4 1 1 2
416 PartnerTech WinIO\\7cfa5e10dff8a99a5d544b011f676bc383991274c693e21e3af40cf6982adb8c Hash deny 4 1 1 2
417 PartnerTech WinIO\\c9b49b52b493b53cd49c12c3fa9553e57c5394555b64e32d1208f5b96a5b8c6e Hash deny 4 1 1 2
418 PartnerTech WinIO\\dc2b92f59fd8d059a58cc0761212f788d7041f708f4bd717d1738de909b4f781 Hash deny 4 1 1 2
419 PassMark DirectIo.sys Hash deny 42 14 10 18
420 PCHunter.sys\\3f20ac5dac9171857fc5791865458fdb6eac4fab837d7eabc42cb0a83cb522fc Hash deny 4 1 1 2
421 phydmaccx64\\f7b3112b9745b766c8359d25e315975d3159935a8ddb3e3035d21ed124a9013f Hash deny 4 1 1 2
422 PhyDMACCx86.sys\\23787eb342fd38da73ce785023176f98304267c6f6fa8a50e718da096c7a7951 Hash deny 2 1 1 0
423 phymemx64 Hash deny 4 1 1 2
424 piddrv.sys Hash deny 4 1 1 2
425 piddrv64.sys Hash deny 4 1 1 2
426 ProtectS.sys Hash deny 4 2 2 0
427 Proxy32.sys Hash deny 2 1 1 0
428 Proxy64.sys Hash deny 2 1 1 0
429 psmounterex\\6a04f0fcb89a7d9810e456b8748d962cccc4caab02795c9cdacaab7f827bc398 Hash deny 2 1 1 0
430 psmounterex\\95ee1707928f81c34fb616ae616691d923a7fce3f906f3fb49db935b5605fe7b Hash deny 2 1 1 0
431 psmounterex\\bf04b9387f78580513821fb90558fb123a20836f2664b93483f31db7f7641bf4 Hash deny 2 1 1 0
432 psmounterex\\e891526a0d3b9d8121657c55384e7c1a80ca96c9577cffb62681c700551ce0d6 Hash deny 2 1 1 0
433 qmbsec.sys\\0c801d381292e0476fb435fcc450b7a8970054cc47230c3123f3b6930d8ad799 Hash deny 4 1 1 2
434 qmbsec.sys\\494cf30f87274942694e1d6a5700466382cf1398ff62a64a654b2e396fff43f4 Hash deny 4 1 1 2
435 qmbsec.sys\\51745c658c506484ed79e2d71862b36351bac95a897ddc41aaeb9ba5bdfb2a37 Hash deny 4 1 1 2
436 qmbsec.sys\\be6c3af76d43d6200a387eab9b57791c87dc3a3e21636b3df68bb34e24eebf89 Hash deny 4 1 1 2
437 Realtek rtport\\6f806a9de79ac2886613c20758546f7e9597db5a20744f7dd82d310b7d6457d0 Hash deny 2 1 1 0
438 Realtek rtport\\74e05c6674f48089c617d66d8231cc5271c94430e80bc346cea0dfee44741476 Hash deny 2 1 1 0
439 Realtek rtport\\8fe429c46fedbab8f06e5396056adabbb84a31efef7f9523eb745fc60144db65 Hash deny 4 1 1 2
440 Realtek rtport\\a29093d4d708185ba8be35709113fb42e402bbfbf2960d3e00fd7c759ef0b94e Hash deny 2 1 1 0
441 Realtek rtport\\c490d6c0844f59fdb4aa850a06e283fbf5e5b6ac20ff42ead03d549d8ae1c01b Hash deny 2 1 1 0
442 Realtek rtport\\e3dbafce5ad2bf17446d0f853aeedf58cc25aa1080ab97e22375a1022d6acb16 Hash deny 2 1 1 0
443 Realtek rtport\\ff322cd0cc30976f9dbdb7a3681529aeab0de7b7f5c5763362b02c15da9657a1 Hash deny 2 1 1 0
444 RTCore\\077aa8ff5e01747723b6d24cc8af460a7a00f30cd3bc80e41cc245ceb8305356 Hash deny 4 1 1 2
445 RTCore\\08828990218ebb4415c1bb33fa2b0a009efd0784b18b3f7ecd3bc078343f7208 Hash deny 4 1 1 2
446 RTCore\\0aca4447ee54d635f76b941f6100b829dc8b2e0df27bdf584acb90f15f12fbda Hash deny 4 1 1 2
447 RTCore\\1c425793a8ce87be916969d6d7e9dd0687b181565c3b483ce53ad1ec6fb72a17 Hash deny 4 1 1 2
448 RTCore\\3ff50c67d51553c08dcb7c98342f68a0f54ad6658c5346c428bdcd1f185569f6 Hash deny 4 1 1 2
449 semav6msr64.sys Hash deny 4 1 1 2
450 sepdrv3.sys\\11cf5a8c3a2cdd8df81e8c3e477bb84b25fb92becb41f35a5d67acaa1466890 Hash deny 4 1 1 2
451 sepdrv3.sys\\17f19350ea6715ce94ca2014bce92a5c07fd752fd06647a8200db6b052468810 Hash deny 4 1 1 2
452 sepdrv3.sys\\1b83be686572c6f0f7d214af7a135978bf5a342e4aafbb01c1a0cf5f6e054863 Hash deny 4 1 1 2
453 sepdrv3.sys\\321104460942bf98c5c248f660e068e5170c16ae8eedfa7acc5bf98471042a4e Hash deny 4 1 1 2
454 sepdrv3.sys\\398853920c10c4d3c685d4222b067e2d7f6b2430adb70577d4e448078de5c64c Hash deny 4 1 1 2
455 sepdrv3.sys\\3fd6a8623394db4eaeabfb7a6e75d8af6f998409de4537045db06391d82b37c8 Hash deny 4 1 1 2
456 sepdrv3.sys\\54fc3cad3fc4d45eaf43b96b175a65879761c996c4e26880064170811b0a11ff Hash deny 4 1 1 2
457 sepdrv3.sys\\94ef5e5188b675da304ac1724655072ec4abc2d48ca545daa7ccfc52ded2d7ae Hash deny 4 1 1 2
458 sepdrv3.sys\\b96ba5c469591f9e545bef4af1719a831c73b71207fad79efd84335c1519f71a Hash deny 4 1 1 2
459 sepdrv3.sys\\f7cb042aaddd24d867c2ac3a5386d736be91f65c47752fef7e93ce4c0e2b8e1e Hash deny 4 1 1 2
460 Shenzhen Moyea Phymem.sys\\0b9a7449bade14983a7520f2d57448823b85a22074ddb48f0e47b9c5442da68b Hash deny 4 1 1 2
461 Shenzhen Moyea Phymem.sys\\0f6801af54ff8bf9a2e1f61bf5cbaeed199af5ec868bfb25859f99aa45d885f0 Hash deny 4 1 1 2
462 Shenzhen Moyea Phymem.sys\\4ec7af309a9359c332d300861655faeceb68bb1cd836dd66d10dd4fac9c01a28 Hash deny 4 1 1 2
463 Shenzhen Moyea Phymem.sys\\d64478376497107c15d948c2d3c86c48bc45833001c6b5c51de05862de57bb02 Hash deny 4 1 1 2
464 superbmc.sys\\1d804efc9a1a012e1f68288c0a2833b13d00eecd4a6e93258ba100aa07e3406f Hash deny 4 1 1 2
465 superbmc.sys\\1deae340bf619319adce00701de887f7434deab4d5547a1742aeedb5634d23c6 Hash deny 4 1 1 2
466 superbmc.sys\\326b53365f8486c78608139cac84619eff90be361f7ade9db70f9867dd94dcc9 Hash deny 4 1 1 2
467 superbmc.sys\\a6f8aa3de5b4aea58eddd45807d722c864d4bc4a38ad573174af864e21f0d526 Hash deny 2 1 1 0
468 superbmc.sys\\c9c60f560440ff16ad3c767ff5b7658d5bda61ea1166efe9b7f450447557136e Hash deny 4 1 1 2
469 superbmc.sys\\ee6bfdf5748fbbf579d6176026626ef39a0673e307c2029f5633e80f0babef54 Hash deny 4 1 1 2
470 t.sys Hash deny 2 1 1 0
471 t3.sys Hash deny 2 1 1 0
472 t7.sys Hash deny 2 1 1 0
473 t8.sys Hash deny 2 1 1 0
474 Tdeio.sys\\1076504a145810dfe331324007569b95d0310ac1e08951077ac3baf668b2a486 Hash deny 4 1 1 2
475 Tdeio.sys\\aa282c3b989a0eca78023347b7b1e1feef7e42edf9fd2bef5d55c66000c99911 Hash deny 4 1 1 2
476 tdklib.sys\\03920ed3f904838b65c2065a338f08ce062c40247539345ea8f4c158efe66634 Hash deny 2 1 1 0
477 TdkLib.sys\\20d0759b3309603ea085ed31a636e42301df7ddcd358584e2ccd6cabf72af7c3 Hash deny 4 2 2 0
478 tdklib.sys\\2695390a8a7448390fe383beb1eee06d582202683f0273d6e72ef39a8cf709e1 Hash deny 2 1 1 0
479 TdkLib.sys\\31e2e5c3290989e8624820cf5af886fd778ee8187fed593f33a6178f65103f37 Hash deny 4 2 2 0
480 TdkLib.sys\\39f137083e6c0200543e1f8d3c074f857d141bdb8c8f09338d48520537b881aa Hash deny 4 2 2 0
481 tdklib.sys\\44a0537e95ebd2d6851f64fab72a41b2f95b5d93729b8978c20350f038ddb8f7 Hash deny 2 1 1 0
482 tdklib.sys\\45a74b2e7ab35dc783375a4603efd961f6dfefaf3c134d453d6b76af94607e74 Hash deny 2 1 1 0
483 tdklib.sys\\4c807bacfcf5c30686e26812ec8d5581a824b82fee7434260c27c33eee2dfbe2 Hash deny 2 1 1 0
484 TdkLib.sys\\4c807bacfcf5c30686e26812ec8d5581a824b82fee7434260c27c33eee2dfbe2 Hash deny 2 1 1 0
485 tdklib.sys\\506f56996fbcd34ff8a27e6948a2e2e21e6dbf42dab6e3a6438402000b969fd1 Hash deny 2 1 1 0
486 TdkLib.sys\\506f56996fbcd34ff8a27e6948a2e2e21e6dbf42dab6e3a6438402000b969fd1 Hash deny 2 1 1 0
487 TdkLib.sys\\5be106b92424b12865338b3f541b3c244dce9693fe15f763316f0c6d6fc073ee Hash deny 4 2 2 0
488 tdklib.sys\\5fbfd7c4ea3db1197ad38d5a945acf6f2f42cb350380cf8ae276bc80b0dedb77 Hash deny 2 1 1 0
489 TdkLib.sys\\5fbfd7c4ea3db1197ad38d5a945acf6f2f42cb350380cf8ae276bc80b0dedb77 Hash deny 2 1 1 0
490 tdklib.sys\\784be598d13257a77da904052260f475d115d45aad3be28e44af2b16b19a840b Hash deny 2 1 1 0
491 TdkLib.sys\\797c1f883d90d25e7fd553624bb16bfd5db24c2658aa0c3c51c715d5833c10fd Hash deny 4 2 2 0
492 TdkLib.sys\\7b2ed5b6f6296cdd3e61a915707355bd5325ce7f5a6fab43b7e1e550277ecaed Hash deny 4 2 2 0
493 tdklib.sys\\a0dba310dc9e89b468523663545e0c3cc82f545d12ddaab234f8903e58ff359a Hash deny 2 1 1 0
494 TdkLib.sys\\aa0c52cebd64a0115c0e7faf4316a52208f738f66a54b4871bd4162eb83dc41a Hash deny 4 2 2 0
495 TdkLib.sys\\cbd4f66ae09797fcd1dc943261a526710acc8dd4b24e6f67ed4a1fce8b0ae31c Hash deny 4 2 2 0
496 tdklib.sys\\dd0bd7b8fae8e8835ba09118a02a06a51e111fccbe16916414844aab91cfeed4 Hash deny 2 1 1 0
497 TdkLib.sys\\dd0bd7b8fae8e8835ba09118a02a06a51e111fccbe16916414844aab91cfeed4 Hash deny 2 1 1 0
498 tdklib.sys\\e34afe0a8c5459d13e7a11f20d62c7762b2a55613aaf6dbeb887e014b5f19295 Hash deny 2 1 1 0
499 TdkLib.sys\\e34afe0a8c5459d13e7a11f20d62c7762b2a55613aaf6dbeb887e014b5f19295 Hash deny 2 1 1 0
500 TestBone.sys Hash deny 2 1 1 0
501 TGSafe.sys Hash deny 2 1 1 0
502 windows-xp-64.sys Hash deny 2 1 1 0
503 windows7-32.sys Hash deny 2 1 1 0
504 windows8-10-32.sys Hash deny 2 1 1 0
505 WindowsKernelExplorer.sys\\455ff5274fbdd19ce1da6fc6725a00752761998759c6bacb9713081f613c1752 Hash deny 2 1 1 0
506 WindowsKernelExplorer.sys\\cee56287a33602d453b6760d1e19081ada0ba9c70becb430512b6e1669dcfff9 Hash deny 2 1 1 0
507 WinFlash64.sys Hash deny 4 1 1 2
508 WinIO\\0bfcf39a3e63bb6ef8afec67965103df1b9803bca31d221a7fd4233972be9e05 Hash deny 4 1 1 2
509 WinIO\\13a38c92606de7bc61960606deb59e1db125fb4efbb8b29ba732e5d3c2dc169c Hash deny 4 1 1 2
510 WinIO\\1f868677f2e6afd63b974908f793307a91329a6a413cfd726e85185507401afb Hash deny 2 1 1 0
511 WinIO\\2e8c28298890f1684be3827bcdb0746a124a0ffe58d1c9a4c361c2e8b13cf735 Hash deny 4 1 1 2
512 WinIO\\385660a65e69b3bf9ac5c2ae4cadbb1e07f366e1807979bf7a915e40e9480f8b Hash deny 4 1 1 2
513 WinIO\\42322b59f75f3ee3f66d080433c01fe024ca9ce5cbd3acac8a98394ac2a0d659 Hash deny 4 1 1 2
514 WinIO\\8d5466ccce64de5beccc373e0c878ca3e624ed78d359f76aae32de4df5afce18 Hash deny 4 1 1 2
515 WinIO\\9ef6eb93e504351d710b88fd5ec68ef2e0b757ea364341e715b0076dc559b54a Hash deny 4 1 1 2
516 WinIO\\9fc29480407e5179aa8ea41682409b4ea33f1a42026277613d6484e5419de374 Hash deny 4 1 1 2
517 WinIO\\be929ae99015fafa0ab55cb475035e8c1359db1b61e00507defc1919a3538385 Hash deny 4 1 1 2
518 WinIO\\d6518cb6dc0cfdfefb9e2210e3de18867748a77153fa11bc7263cdbc58919815 Hash deny 4 1 1 2
519 WinIO\\db4a5b87db97167c70e98014a12ac324866cf643cee65d3b3cda0b33add34d2f Hash deny 4 1 1 2
520 WinIO\\f4acfebd83a351029dd812a0e40b44f5362f31ae80b6ae0b2fa2241687d34912 Hash deny 4 1 1 2
521 WinRing0.sys Hash deny 4 1 1 2
522 WinRing0.sys\\32df55a6f917c549b1bf7b73a94c7386012265e87992ee065161a0a51e35ae57 Hash deny 1 0 0 0
523 WinRing0.sys\\32df55a6f917c549b1bf7b73a94c7386012265e87992ee065161a0a51e35ae57 Hash deny 1 0 0 0
524 WinRing0.Sys\\4448beff8366e42e3393e8c7f8261aee0b0340356c31aa3b97de07452ae01888 Hash deny 2 0 0 0
525 WinRing0.sys\\489c0e3a8d70037eba46810c0d1c93a10e2796d809eb35023e5977d6902cb744 Hash deny 1 0 0 0
526 WinRing0.sys\\9093340be0ab932fa49edd81e9da50914af3e095059908137c49dee991283b81 Hash deny 1 0 0 0
527 WinRing0.sys\\a3e3cdbd8c781960a77efb5655a6b7c3bac8d5926aaf687108a57d99b78538fc Hash deny 1 0 0 0
528 WinRing0.sys\\b88fbe1ca78c07ef2be388e0db4255aa9b7d5661eaa7f3ff2651710133b08fb3 Hash deny 1 0 0 0
529 WinRing0.sys\\fe9f25312476056f7c7b731eedc468581cb3224c0b9a4e23e0723a906977f874 Hash deny 1 0 0 0
530 WinRing0_1_2_2.sys\\82b30461dbf40ac15fce6a83b9bad2ebd05b27dea1b784eaa096422fe8927b7b Hash deny 1 0 0 0
531 WinRing0a64.sys\\3135ec80fcfb32d2482b1d93d131b611b71765cd3657de615a5bcf73e3bb46b0 Hash deny 1 0 0 0
532 WinRing0a64.sys\\de288e320bc3d20b4782678678a95ce93d0244b8be87a2797b0a7419b99ab62c Hash deny 2 0 0 0
533 WinRing0x64.sys\\14d00976162a5d3238d183704fd84b50c3c5dcc762cab3c8adb5faf0a3caab99 Hash deny 1 0 0 0
534 WinRing0x64.sys\\2292fd8d30074e5214f408a98f868a1b97d4dd8959778f20f63820d919a33904 Hash deny 1 0 0 0
535 WinRing0x64.sys\\394a34adfb3dcc2ab37fead7592a25531d625582cebe6fa687913d8900e79b76 Hash deny 1 0 0 0
536 WinRing0x64.sys\\47eaebc920ccf99e09fc9924feb6b19b8a28589f52783327067c9b09754b5e84 Hash deny 1 0 0 0
537 WinRing0x64.sys\\53d025d0405d740b9de824b76d963f662b81ec1ae2f83a21d1b1697cede7ba5f Hash deny 1 0 0 0
538 WinRing0x64.sys\\76290da6a4b67494054a967231d8593bcc8db3f68b96f599680bbdc4aa2fcee0 Hash deny 1 0 0 0
539 WinRing0x64.sys\\7a54ddc926565478ce14d813cedbbbcadcb469c5 Hash deny 1 0 0 0
540 WinRing0x64.sys\\94bccd8dcad88b96d3b2c36b72bd818ec05ed518bf3b73a0a3e9b4f8bf2d31b8 Hash deny 1 0 0 0
541 WinRing0x64.sys\\ebd797feaf27210fc4edc042f33fe01bbfa7a8ffaafe0a62116e8205d8bc0c48 Hash deny 1 0 0 0
542 WinRing0x64.sys\\ec844953cca50e1d2c86b46a93f295165a9e745145ceb0ebb21f54f2093bb21f Hash deny 1 0 0 0
543 wsftprm.sys\\160a6619e5b4ee9e9bd4344f55115748e35e1635a23dbee49ad2a810cba40fa1 Hash deny 2 0 0 0
544 wsftprm.sys\\2e763aae36ef0897d13c262ebc0b53b99acc61268e4e71a6347d276f978a676 Hash deny 2 0 0 0
545 wsftprm.sys\\e20381b3c01bc1df17a1dba6926a2a014baccd76273a49e03a6b947a28477155 Hash deny 2 0 0 0
546 WYProxy32.sys Hash deny 2 1 1 0
547 WYProxy64.sys Hash deny 2 1 1 0
548 YY_DhKernel\\80cbba9f404df3e642f22c476664d63d7c229d45d34f5cd0e19c65eb41becec3 Hash deny 4 1 1 2
549 YY_DhKernel\\bb50818a07b0eb1bd317467139b7eb4bad6cd89053fecdabfeae111689825955 Hash deny 4 1 1 2
550 YY_DhKernel\\dcd026fd2ff8d517e2779d67b3d2d5f9a7aa39f19c66fa8ff2cab66d5c6461c6 Hash deny 4 1 1 2
"""

filename_rules_raw = """
1 kprocesshacker.sys FileRule kprocesshacker.sys 0.0.0.0 3.1.65535.65535
2 System Mechanic CVE-2018-5701 amp.sys 0.0.0.0 5.4.11.1
3 Asus Memory Mapping Driver asmmap.sys 0.0.0.0 65535.65535.65535.65535
4 Asus Memory Mapping Driver asmmap64.sys 0.0.0.0 65535.65535.65535.65535
5 Cheat Engine Driver dbk32.sys 0.0.0.0 65535.65535.65535.65535
6 Cheat Engine Driver dbk64.sys 0.0.0.0 65535.65535.65535.65535
7 gdrv.sys gdrv.sys 0.0.0.0 65535.65535.65535.65535
8 Kaspersky klmd.sys FileRule klmd.sys 0.0.0.0 2.13.0.10
9 PCHunter Driver PCHunter.sys 0.0.0.0 65535.65535.65535.65535
10 PCHunter Driver ■■■■ 0.0.0.0 65535.65535.65535.65535
11 Phymemx64 Memory Mapping Driver phymemx64.sys 0.0.0.0 65535.65535.65535.65535
"""

hash_entries = []
pattern = re.compile(r'^(\d+)\s+(.*?)\s+Hash deny\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)', re.MULTILINE)

for match in pattern.finditer(raw_data):
    idx, label, rules, sha1_cnt, sha256_cnt, page_cnt = match.groups()
    
    # Extract embedded SHA-256 or SHA-1 hash if present in label path
    extracted_hash = ""
    hash_match = re.search(r'\\([a-fA-F0-9]{40,64})$', label)
    if hash_match:
        extracted_hash = hash_match.group(1)
        
    hash_entries.append({
        "id": int(idx),
        "label": label.strip(),
        "rule_type": "Hash deny",
        "rules": int(rules),
        "sha1_count": int(sha1_cnt),
        "sha256_count": int(sha256_cnt),
        "page_hashes_count": int(page_cnt),
        "extracted_hash": extracted_hash
    })

filename_entries = []
fn_pattern = re.compile(r'^(\d+)\s+(.*?)\s+([\w\.\-]+)\s+([\d\.]+)\s+([\d\.]+)', re.MULTILINE)
for match in fn_pattern.finditer(filename_rules_raw):
    idx, friendly, fname, min_v, max_v = match.groups()
    filename_entries.append({
        "id": int(idx),
        "friendly_name": friendly.strip(),
        "filename": fname.strip(),
        "min_version": min_v.strip(),
        "max_version": max_v.strip()
    })

dataset = {
    "metadata": {
        "policy_name": "Microsoft Windows Driver Policy",
        "policy_version": "10.0.29545.0",
        "total_deny_file_rules": 1713,
        "filename_deny_rules": len(filename_entries),
        "hash_based_deny_rules": 1702,
        "distinct_hash_labels": len(hash_entries)
    },
    "filename_deny_rules": filename_entries,
    "hash_deny_rules": hash_entries
}

os.makedirs("data", exist_ok=True)

with open("data/microsoft_blocked_driver_list.json", "w") as f:
    json.dump(dataset, f, indent=2)

with open("data/microsoft_blocked_driver_list.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["id", "label", "rule_type", "rules", "sha1_count", "sha256_count", "page_hashes_count", "extracted_hash"])
    for item in hash_entries:
        writer.writerow([
            item["id"], item["label"], item["rule_type"], item["rules"],
            item["sha1_count"], item["sha256_count"], item["page_hashes_count"], item["extracted_hash"]
        ])

print(f"Parsed {len(hash_entries)} hash rules and {len(filename_entries)} filename rules successfully.")
