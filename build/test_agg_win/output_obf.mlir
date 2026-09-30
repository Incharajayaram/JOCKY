module attributes {dlti.dl_spec = #dlti.dl_spec<!llvm.ptr<270> = dense<32> : vector<4xi64>, !llvm.ptr<271> = dense<32> : vector<4xi64>, !llvm.ptr<272> = dense<64> : vector<4xi64>, i64 = dense<64> : vector<2xi64>, f80 = dense<128> : vector<2xi64>, !llvm.ptr = dense<64> : vector<4xi64>, i1 = dense<8> : vector<2xi64>, i8 = dense<8> : vector<2xi64>, i16 = dense<16> : vector<2xi64>, i32 = dense<32> : vector<2xi64>, f16 = dense<16> : vector<2xi64>, f64 = dense<64> : vector<2xi64>, f128 = dense<128> : vector<2xi64>, "dlti.endianness" = "little", "dlti.mangling_mode" = "w", "dlti.legal_int_widths" = array<i32: 8, 16, 32, 64>, "dlti.stack_alignment" = 128 : i64>, llvm.module_asm = [], llvm.target_triple = "x86_64-w64-windows-gnu"} {
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
  llvm.mlir.global private @".str.0.enc"("r\0Dt\11fK\\\1F-'0S,,SGV\0Ab9SL\E7@\F1\E7=QHb!'0\EA\E8\F632%\148 .\E1\0E0\0B\0B-\1C\E1\C9\FC\E7") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.1.enc"("cfvhm{`\D9qCPki\7FLVNZLtGd\7F|\7FCWV\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.2.enc"(dense<77> : tensor<1xi8>) {addr_space = 0 : i32} : !llvm.array<1 x i8>
  llvm.mlir.global private @".str.3.enc"("\09\0B\09\11o\07b+N-WO]*#\09r\E4 9Y*7\\]#7(\FD\04=(7-\05\07.(+<\03\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.4.enc"("\09\0B\09\11o\07b+}NTRUQ';P>i^R\FBS\\\22#7(\FD\176$9\EAU\C4\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.5.enc"("\09\0B\09\11o\09b+N-WO]*#\09r\E4 U%@SZ'\1B\F28/\0E)'3\EA()$39'\09$\E8\F7\EB\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.6.enc"("cfvhm{`\D9qqohhz{hzWp\7FEZzy|{VOO5") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.7.enc"("\09\0B\09\11o\07b+xPRH^_]@\11k\CE\EB6D01\\]6/15") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.8.enc"("\09\0B\09\11o\07b+}NTRUQ';P>i^R\FBS\\\22#7(\FD\176$9\EA()$39'\09$\F2@\CF\1F") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.9.enc"("\09\0B\09\11o\09b+|N:T\1C}\EC\09\22M\0E%Y-0\E16],/<\14(.6&/\E2\F0(\08?\148\F2)<\091<'6.\03\CA\05\1B<<\0E\07\1C\0E\FE") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.10.enc"("A_]!7\10\1C6\EF\E3\14\EC\0A\EA\10\FB\EB\F2\D4\E7\F7\0E*1_Z3(\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.11.enc"("kLH#Y(\07m_OR^]]UT%GgPR^]\\L&\ECA<\07!&8)4\22-\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.12.enc"("j\1Du|]D[v)\22b@QK*U\1FLk*_0LR\\#7P\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.13.enc"("\09\0B\09\11\04\1E\07+O$K.%WPL\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.14.enc"("kLH#Y(\07t@UK] SVT!$kW):+^\\]Y8J\1C-'K8\C4\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.15.enc"("r\0Dt\11gFZjEHTQ\1CKQMTF T*8^]\22)KP4\099\E7\FA\E0\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.16.enc"("\09\0B\09\11o\07b+cNJWH\1CXD$@d\EBPN &_[;\FE\FD5") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.17.enc"("\09UP]Y/'") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.18.enc"("\09\0B\09\11o\09b+cNJWH\1CPD%\0Ab^)A[\E1_Z=+Ia9\E5\F4)>:-0\0F\14\13'1\E11*\0E3$?*\03\FC\FD\E0\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.19.enc"("cfvhm{rVztryNsJdNZLtGd\7F|\7FCWV\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.20.enc"("YC@\E0cA5l-HUJ{!SGUG\13:\12F$*%\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.21.enc"("\09\0B\09\11\04\1E\07+zN=JHI[MX@c\EBZ-\\R\E9\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.22.enc"("\09\0B\09\11o\07b+cNJWH\1CVD&@l^%G\E7^]\\&S<\09--\14") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.23.enc"("r\0Dt\11g]@sGIM\1C_IPOXO\159%7^\\]\E7NQ>\16T%-\E0\E0\E0\10") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.24.enc"("\09\0B\09\11o\07b+}NTRUQ\1AHPKhPX\FB+\\\E9\E7\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.25.enc"("j\1Du|]D[v)\22b@QK*U\1FLk*_0L \\]0S25") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.26.enc"("r\0Dt\11pKFoGIM\1C__]ATN *SA%^$0,+!\1E+'\FA\E0\E0\16") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.27.enc"("A_]!7\10\1C6]PATQP\10B_N\10^]A+\1C67NQ<\19\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.28.enc"("kLH#Y(\07j_RVWPy&DZMj\0B") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.29.enc"("r\0Dt\11eRKpC#:KJQ\1AkHgrw\E4G1^5 ,\E4>\1D!&:\EA&#1;&>\0D\E7\E8\F7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.30.enc"("\09\0B\09\11x(^\1D[-1\1C I\1A=#1\D6\EB\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.31.enc"("\09\0B~1") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.32.enc"("\06+") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.33.enc"("t\0B)") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.34.enc"("\09\0B\09\11\04\1Eh2s\13ri}pwm\11\05 s%A[]&\F1\F2\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.35.enc"("\09\0B\09\11\04\1Eh2s\13jW\22W]B\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.36.enc"("\09\0B\09\11\04\1Eh2s\13kDLhqfE\0ArpV`E~FC\F2\17\FD|-;:-&\F61&8;9\0C\F24?\03<2&5/\DF") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.37.enc"("r\02t\11fu|}z\13AT]WP\09RGm;PD+Z\E3\18\F2?.\1E*,\F4\0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.38.enc"("\09\0B\09\11\04\1Eh,s\13k$,HQF%\0A\14P77\E7'\22\\N/1!\E0=&1# /\E7%;\02\09\F2-\0F4\0F6\1A\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.39.enc"("\09\0B\09\11\04\1Eh,s\13rI]P\1AOPAlPX\FB\12\E1'9K:0\03\E0';>\EA)>$&<\0B3.*\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.40.enc"("r\04t\11rK\07Iwn\\p\1CP,F'M\0E:\E485&Z[3&I\1A\E0&:\EA>.)\16\FF!\12>+7\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.41.enc"("\14\14\14\11~k`Rw\13]KJPQ<\22\0AnP7D&3 _\F2E5\16)'\F48\C5\F6\E5\E7/\12\159\07 \01463\E8\CD\D6\FC\EA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.42.enc"("k\\@EX\1E~O\14\13.") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.43.enc"("cfvhm{a@gojyupAY}i\7Fp|nsEFY\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.44.enc"("r\0Dt\11eRKpC#:KJQ\1A=^\0Al^%G\E7 \22&J/1\D5'$:(#+=\11>\14\13 (\F7\EB\F5\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.45.enc"("r\04t\11rK\07j_RVWP\1C]D_HiV\E485&Z[3&I\1A\14\E9!;# /\E7''89=4101\FD.1'\0B\08\04\1D9\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.46.enc"("r\04t\11z]^w[W\0E K\1CQK%Ii]\E4L\\%&[\0E\E4>`*== ?!&:\FF\15\13\09*4\00\03\FDZA\F2\04>=\11#>\07\E0") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.47.enc"("r\02t\11gK]mGV;.] SD_\0Ao QL&3*\F1\12") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.48.enc"("\09\0B\09\11gb}+{IJ,KWP=\EB\0A\00") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.49.enc"("\09\0B\09\11qK[pB\13^S T\E4\091") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.50.enc"("\09\0B\09\11fQ^wZ\E5\0E<") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.51.enc"("r\0Dt\11WRF\19*HTQ\1CS\229]Gi'%7^\\]\E7,/.\1A!;7\22\EA7($&>\D4\E7\E8\01") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.52.enc"("\09\0B\09\11\11\1EaDaAj\1CP*S?T$ *\\8^_\E9\E7\FB\E41\03)?1\04\05\F6)=\FF&\0B!.#<>2\FD'\1C/\04\18\E7") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.53.enc"("\09\0B\09\11\11\1Exp,IKH\1CS\229]Gi'%7^\\]\E7&/>\1D*&%=/\07\F0$\0D'\13!3#)0\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.54.enc"("\09\0B\09\11\11\1ErW\03W0K\22SP\09%B\0EP%7\E7&0675.b-' \0A") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.55.enc"("\09\0B\09\11\11\1EzcXHR ._&F^@\D6\EBGg}\1D\E3ChU\FD\09=':-&\E2\F0[&\11\09 \0C-\FD\020? ? \06\EA") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.56.enc"("\09\0B\09\11\11\1Eev,TT-U]-\F3\11kkX4-ZY&]-S#\1A\E0(8-+ =\17\FF;\14><503\1D") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.57.enc"("r\02t\11eJS+-X1 QK-\09X@i']8S^) 6\E4<\1F,\E9&-+*9\07") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.58.enc"("r\0Dt\11yVEtB'0S WQG\11MjW4N^_7\F1\F2\04") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.59.enc"("r\0Dt\11eQKs\1E'UUQV\E4\091") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.60.enc"("r\0Dt\11TFF\1A[\13\FC\E6\1C\7F&=TE\10']A$\E1ALiZQ\D5-1$&!!<$3?\15'\F2 5<43\E6\F8\E1\DF") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.61.enc"("r\02t\11fu|}z\13K$,HQF%I\14\\SA\E706&=/.\04\2228\EA\E7\F6+8\09>\0F!\F2.>>0\0C\1B\F2 =\06\04#>\05\04\E6") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.62.enc"("r\04t\11fu|}z\13AT]WP\09T2hT)*+Z'\E7\0F\E4<aT\E90\04#8-\11\08\E0?'3\1F<4):*>.\DF") {addr_space = 0 : i32}
  llvm.mlir.global private @".str.63.enc"("\14\14\14\11VA0p_-AT\1C]RNX@ 9Y8[.\E3%I6\FD\19-98'3%-=3\E0\C7\F2\FF\01") {addr_space = 0 : i32}
  llvm.mlir.global external @g_50877ae9() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_a272e225() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
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
  llvm.mlir.global external @g_95bd4817() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @g_b7bfea9b() {addr_space = 0 : i32} : !llvm.ptr {
    %0 = llvm.mlir.zero : !llvm.ptr
    llvm.return %0 : !llvm.ptr
  }
  llvm.mlir.global external @c2_config_fresh(false) {addr_space = 0 : i32} : i1
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
  llvm.func @jocky_spoof_call(!llvm.ptr, i64, i64, i64, i64) -> i64
  llvm.func @jocky_spoof_syscall(i32, i64, i64, i64, i64) -> i64
  llvm.func @jocky_byovd_load(!llvm.ptr, !llvm.ptr, !llvm.ptr) -> i1
  llvm.func @jocky_byovd_unload(!llvm.ptr)
  llvm.func @jocky_driver_read_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_write_phys(!llvm.ptr, i64, !llvm.ptr, i32) -> i1
  llvm.func @jocky_driver_map_kernel(!llvm.ptr, i64, i32) -> !llvm.ptr
  llvm.func @jocky_disable_edr_callbacks(!llvm.ptr) -> i32
  llvm.func @jocky_disable_etw(!llvm.ptr) -> i1
  llvm.func @jocky_elevate_token(!llvm.ptr, i32) -> i1
  llvm.func @jocky_process_hollow(!llvm.ptr, !llvm.ptr, i32) -> i1
  llvm.func @jocky_rdll_inject(i32, !llvm.ptr, i32) -> i1
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
  llvm.func @f_0218a827() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @g_9db24be5 : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @g_95bd4817 : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %16 = llvm.mlir.constant(false) : i1
    %17 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %18 = llvm.mlir.constant(true) : i1
    %19 = llvm.mlir.addressof @c2_config_fresh : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<54 x i8>
    llvm.call @println(%23) : (!llvm.ptr) -> ()
    %24 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %25 = llvm.call @f_f8b7e158(%24) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %25, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %26 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %28 = llvm.icmp "ne" %26, %27 : !llvm.ptr
    llvm.cond_br %28, ^bb1, ^bb4
  ^bb1:  // pred: ^bb0
    %29 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<42 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %31 = llvm.call @f_b9127cd9(%30) : (!llvm.ptr) -> i1
    llvm.cond_br %31, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %32 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    llvm.store %18, %19 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.return %18 : i1
  ^bb3:  // pred: ^bb1
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb0, ^bb3
    %33 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    %35 = llvm.call @f_f8b7e158(%34) : (!llvm.ptr) -> !llvm.ptr
    llvm.store %35, %22 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %36 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %37 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %38 = llvm.icmp "ne" %36, %37 : !llvm.ptr
    llvm.cond_br %38, ^bb5, ^bb8
  ^bb5:  // pred: ^bb4
    %39 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    llvm.call @println(%39) : (!llvm.ptr) -> ()
    %40 = llvm.load %22 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %41 = llvm.call @f_b9127cd9(%40) : (!llvm.ptr) -> i1
    llvm.cond_br %41, ^bb6, ^bb7
  ^bb6:  // pred: ^bb5
    %42 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<46 x i8>
    llvm.call @println(%42) : (!llvm.ptr) -> ()
    llvm.store %18, %19 {alignment = 1 : i64} : i1, !llvm.ptr
    llvm.return %18 : i1
  ^bb7:  // pred: ^bb5
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb4, ^bb7
    %43 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<62 x i8>
    llvm.call @println(%43) : (!llvm.ptr) -> ()
    %44 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %44, %11 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %45 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.store %45, %13 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %46 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %46, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %16 : i1
  }
  llvm.func @f_f8b7e158(%arg0: !llvm.ptr) -> !llvm.ptr {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %4 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %4 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %5 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %6 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %7 = llvm.call @jocky_str_concat(%5, %6) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%7) : (!llvm.ptr) -> ()
    %8 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.return %8 : !llvm.ptr
  }
  llvm.func @f_b9127cd9(%arg0: !llvm.ptr) -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @g_9db24be5 : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @g_95bd4817 : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %8 = llvm.mlir.constant(true) : i1
    %9 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %arg0, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %10 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %10, %3 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %11 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.store %11, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %12 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %12, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %8 : i1
  }
  llvm.func @f_b610cf36() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %11 = llvm.mlir.constant(true) : i1
    %12 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %14 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %15 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %16 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<35 x i8>
    llvm.call @println(%16) : (!llvm.ptr) -> ()
    %17 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %18 = llvm.call @fs_exists(%17) : (!llvm.ptr) -> i1
    llvm.cond_br %18, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %19 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %20 = llvm.call @fs_file_size(%19) : (!llvm.ptr) -> i64
    llvm.store %20, %14 {alignment = 8 : i64} : i64, !llvm.ptr
    %21 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    %22 = llvm.load %14 {alignment = 8 : i64} : !llvm.ptr -> i64
    %23 = llvm.call @string(%22) : (i64) -> !llvm.ptr
    %24 = llvm.call @jocky_str_concat(%21, %23) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %25 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<7 x i8>
    %26 = llvm.call @jocky_str_concat(%24, %25) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%26) : (!llvm.ptr) -> ()
    llvm.return %11 : i1
  ^bb2:  // pred: ^bb0
    %27 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<56 x i8>
    llvm.call @println(%27) : (!llvm.ptr) -> ()
    %28 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    %29 = llvm.getelementptr %6[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<26 x i8>
    %30 = llvm.call @jocky_str_concat(%28, %29) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.store %30, %15 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %31 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %32 = llvm.load %15 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %33 = llvm.call @jocky_str_concat(%31, %32) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<33 x i8>
    llvm.call @println(%34) : (!llvm.ptr) -> ()
    %35 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %35, %10 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %11 : i1
  }
  llvm.func @f_97a724e0() -> i1 {
    %0 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %3 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(true) : i1
    %5 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<37 x i8>
    llvm.call @println(%5) : (!llvm.ptr) -> ()
    %6 = llvm.getelementptr %2[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %7 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    %8 = llvm.call @jocky_str_concat(%6, %7) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%8) : (!llvm.ptr) -> ()
    llvm.return %4 : i1
  }
  llvm.func @f_c1c92895() -> i1 {
    %0 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(0 : i32) : i32
    %2 = llvm.mlir.constant(false) : i1
    %3 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @g_9db24be5 : !llvm.ptr
    %6 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %7 = llvm.mlir.addressof @g_95bd4817 : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @g_b7bfea9b : !llvm.ptr
    %10 = llvm.mlir.constant(true) : i1
    %11 = llvm.getelementptr %0[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<30 x i8>
    %12 = llvm.call @fs_exists(%11) : (!llvm.ptr) -> i1
    llvm.cond_br %12, ^bb1, ^bb2
  ^bb1:  // pred: ^bb0
    %13 = llvm.getelementptr %3[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<36 x i8>
    llvm.call @println(%13) : (!llvm.ptr) -> ()
    %14 = llvm.getelementptr %4[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<31 x i8>
    llvm.store %14, %5 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %15 = llvm.getelementptr %6[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<20 x i8>
    llvm.store %15, %7 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %16 = llvm.getelementptr %8[%1, %1] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<29 x i8>
    llvm.store %16, %9 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    llvm.return %10 : i1
  ^bb2:  // pred: ^bb0
    llvm.return %2 : i1
  }
  llvm.func @f_fe69c41f() -> i1 {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @g_d122d020 : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(0 : i64) : i64
    %7 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %8 = llvm.mlir.constant(false) : i1
    %9 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(1 : i64) : i64
    %17 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(true) : i1
    %20 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %21 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    %22 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %23 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    %24 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%24) : (!llvm.ptr) -> ()
    %25 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<21 x i8>
    %26 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %27 = llvm.call @array_len(%26) : (!llvm.ptr) -> i64
    %28 = llvm.call @string(%27) : (i64) -> !llvm.ptr
    %29 = llvm.call @jocky_str_concat(%25, %28) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%30) : (!llvm.ptr) -> ()
    %31 = llvm.alloca %0 x i32 {alignment = 4 : i64} : (i32) -> !llvm.ptr
    llvm.store %2, %31 {alignment = 4 : i64} : i32, !llvm.ptr
    %32 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %33 = llvm.call @array_len(%32) : (!llvm.ptr) -> i64
    %34 = llvm.alloca %0 x i64 {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.store %6, %34 {alignment = 8 : i64} : i64, !llvm.ptr
    %35 = llvm.alloca %0 x !llvm.ptr {alignment = 8 : i64} : (i32) -> !llvm.ptr
    llvm.br ^bb1
  ^bb1:  // 2 preds: ^bb0, ^bb9
    %36 = llvm.load %34 {alignment = 8 : i64} : !llvm.ptr -> i64
    %37 = llvm.icmp "slt" %36, %33 : i64
    llvm.cond_br %37, ^bb2, ^bb10
  ^bb2:  // pred: ^bb1
    %38 = llvm.bitcast %32 : !llvm.ptr to !llvm.ptr
    %39 = llvm.getelementptr %38[%36] : (!llvm.ptr, i64) -> !llvm.ptr, !llvm.ptr
    %40 = llvm.load %39 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %40, %35 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %41 = llvm.load %35 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %42 = llvm.bitcast %41 : !llvm.ptr to !llvm.ptr
    %43 = llvm.getelementptr %42[%2] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %44 = llvm.load %43 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %44, %20 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %45 = llvm.load %35 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %46 = llvm.bitcast %45 : !llvm.ptr to !llvm.ptr
    %47 = llvm.getelementptr %46[%0] : (!llvm.ptr, i32) -> !llvm.ptr, !llvm.ptr
    %48 = llvm.load %47 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    llvm.store %48, %21 {alignment = 8 : i64} : !llvm.ptr, !llvm.ptr
    %49 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<4 x i8>
    %50 = llvm.load %31 {alignment = 4 : i64} : !llvm.ptr -> i32
    %51 = llvm.add %50, %0 : i32
    %52 = llvm.sext %51 : i32 to i64
    %53 = llvm.call @string(%52) : (i64) -> !llvm.ptr
    %54 = llvm.call @jocky_str_concat(%49, %53) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %55 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<2 x i8>
    %56 = llvm.call @jocky_str_concat(%54, %55) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %57 = llvm.load %4 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %58 = llvm.call @array_len(%57) : (!llvm.ptr) -> i64
    %59 = llvm.call @string(%58) : (i64) -> !llvm.ptr
    %60 = llvm.call @jocky_str_concat(%56, %59) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %61 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<3 x i8>
    %62 = llvm.call @jocky_str_concat(%60, %61) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    %63 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %64 = llvm.call @jocky_str_concat(%62, %63) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%64) : (!llvm.ptr) -> ()
    %65 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %66 = llvm.call @byovd_load_driver(%65) : (!llvm.ptr) -> i32
    llvm.store %66, %22 {alignment = 4 : i64} : i32, !llvm.ptr
    %67 = llvm.load %22 {alignment = 4 : i64} : !llvm.ptr -> i32
    %68 = llvm.icmp "sgt" %67, %2 : i32
    llvm.cond_br %68, ^bb3, ^bb7
  ^bb3:  // pred: ^bb2
    %69 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %70 = llvm.load %22 {alignment = 4 : i64} : !llvm.ptr -> i32
    %71 = llvm.sext %70 : i32 to i64
    %72 = llvm.call @string(%71) : (i64) -> !llvm.ptr
    %73 = llvm.call @jocky_str_concat(%69, %72) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%73) : (!llvm.ptr) -> ()
    %74 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %75 = llvm.load %21 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %76 = llvm.call @jocky_str_concat(%74, %75) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%76) : (!llvm.ptr) -> ()
    %77 = llvm.load %22 {alignment = 4 : i64} : !llvm.ptr -> i32
    %78 = llvm.call @byovd_test_exploit(%77) : (i32) -> i32
    llvm.store %78, %23 {alignment = 4 : i64} : i32, !llvm.ptr
    %79 = llvm.load %23 {alignment = 4 : i64} : !llvm.ptr -> i32
    %80 = llvm.icmp "eq" %79, %2 : i32
    llvm.cond_br %80, ^bb4, ^bb5
  ^bb4:  // pred: ^bb3
    %81 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<52 x i8>
    llvm.call @println(%81) : (!llvm.ptr) -> ()
    %82 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%82) : (!llvm.ptr) -> ()
    %83 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    %84 = llvm.load %20 {alignment = 8 : i64} : !llvm.ptr -> !llvm.ptr
    %85 = llvm.call @jocky_str_concat(%83, %84) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%85) : (!llvm.ptr) -> ()
    llvm.return %19 : i1
  ^bb5:  // pred: ^bb3
    %86 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<50 x i8>
    llvm.call @println(%86) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // pred: ^bb5
    llvm.br ^bb8
  ^bb7:  // pred: ^bb2
    %87 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%87) : (!llvm.ptr) -> ()
    llvm.br ^bb8
  ^bb8:  // 2 preds: ^bb6, ^bb7
    %88 = llvm.load %31 {alignment = 4 : i64} : !llvm.ptr -> i32
    %89 = llvm.add %88, %0 : i32
    llvm.store %89, %31 {alignment = 4 : i64} : i32, !llvm.ptr
    llvm.br ^bb9
  ^bb9:  // pred: ^bb8
    %90 = llvm.add %36, %16 : i64
    llvm.store %90, %34 {alignment = 8 : i64} : i64, !llvm.ptr
    llvm.br ^bb1
  ^bb10:  // pred: ^bb1
    %91 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%91) : (!llvm.ptr) -> ()
    %92 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<45 x i8>
    llvm.call @println(%92) : (!llvm.ptr) -> ()
    llvm.return %8 : i1
  }
  llvm.func @main() {
    %0 = llvm.mlir.constant(1 : i32) : i32
    %1 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %2 = llvm.mlir.constant(0 : i32) : i32
    %3 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %4 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %5 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %6 = llvm.mlir.constant(true) : i1
    %7 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %8 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %9 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %10 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %11 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %12 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %13 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %14 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %15 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %16 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %17 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %18 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %19 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %20 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %21 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %22 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %23 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %24 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %25 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %26 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %27 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %28 = llvm.alloca %0 x i1 {alignment = 1 : i64} : (i32) -> !llvm.ptr
    %29 = llvm.getelementptr %1[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%29) : (!llvm.ptr) -> ()
    %30 = llvm.getelementptr %3[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<11 x i8>
    %31 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %32 = llvm.call @jocky_str_concat(%30, %31) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%32) : (!llvm.ptr) -> ()
    %33 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%33) : (!llvm.ptr) -> ()
    %34 = llvm.call @f_0218a827() : () -> i1
    llvm.store %34, %27 {alignment = 1 : i64} : i1, !llvm.ptr
    %35 = llvm.load %27 {alignment = 1 : i64} : !llvm.ptr -> i1
    %36 = llvm.xor %35, %6 : i1
    llvm.cond_br %36, ^bb1, ^bb4
  ^bb1:  // pred: ^bb0
    %37 = llvm.getelementptr %7[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%37) : (!llvm.ptr) -> ()
    %38 = llvm.call @f_c1c92895() : () -> i1
    %39 = llvm.xor %38, %6 : i1
    llvm.cond_br %39, ^bb2, ^bb3
  ^bb2:  // pred: ^bb1
    %40 = llvm.getelementptr %8[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<57 x i8>
    llvm.call @println(%40) : (!llvm.ptr) -> ()
    llvm.br ^bb3
  ^bb3:  // 2 preds: ^bb1, ^bb2
    llvm.br ^bb4
  ^bb4:  // 2 preds: ^bb0, ^bb3
    %41 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%41) : (!llvm.ptr) -> ()
    %42 = llvm.call @f_b610cf36() : () -> i1
    %43 = llvm.xor %42, %6 : i1
    llvm.cond_br %43, ^bb5, ^bb6
  ^bb5:  // pred: ^bb4
    %44 = llvm.getelementptr %9[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<58 x i8>
    llvm.call @println(%44) : (!llvm.ptr) -> ()
    llvm.br ^bb6
  ^bb6:  // 2 preds: ^bb4, ^bb5
    %45 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%45) : (!llvm.ptr) -> ()
    %46 = llvm.call @f_97a724e0() : () -> i1
    %47 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%47) : (!llvm.ptr) -> ()
    %48 = llvm.getelementptr %10[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    llvm.call @println(%48) : (!llvm.ptr) -> ()
    %49 = llvm.getelementptr %11[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<19 x i8>
    %50 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %51 = llvm.call @jocky_str_concat(%49, %50) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%51) : (!llvm.ptr) -> ()
    %52 = llvm.getelementptr %12[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %53 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %54 = llvm.call @jocky_str_concat(%52, %53) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%54) : (!llvm.ptr) -> ()
    %55 = llvm.getelementptr %13[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<12 x i8>
    %56 = llvm.getelementptr %4[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<27 x i8>
    %57 = llvm.call @jocky_str_concat(%55, %56) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%57) : (!llvm.ptr) -> ()
    %58 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%58) : (!llvm.ptr) -> ()
    %59 = llvm.getelementptr %14[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%59) : (!llvm.ptr) -> ()
    %60 = llvm.getelementptr %15[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<54 x i8>
    llvm.call @println(%60) : (!llvm.ptr) -> ()
    %61 = llvm.getelementptr %16[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%61) : (!llvm.ptr) -> ()
    %62 = llvm.getelementptr %17[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<34 x i8>
    llvm.call @println(%62) : (!llvm.ptr) -> ()
    %63 = llvm.getelementptr %18[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<53 x i8>
    llvm.call @println(%63) : (!llvm.ptr) -> ()
    %64 = llvm.getelementptr %19[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<47 x i8>
    llvm.call @println(%64) : (!llvm.ptr) -> ()
    %65 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%65) : (!llvm.ptr) -> ()
    %66 = llvm.getelementptr %20[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<38 x i8>
    llvm.call @println(%66) : (!llvm.ptr) -> ()
    %67 = llvm.getelementptr %21[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<28 x i8>
    %68 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %69 = llvm.call @jocky_str_concat(%67, %68) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%69) : (!llvm.ptr) -> ()
    %70 = llvm.getelementptr %22[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<17 x i8>
    %71 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    %72 = llvm.call @jocky_str_concat(%70, %71) : (!llvm.ptr, !llvm.ptr) -> !llvm.ptr
    llvm.call @println(%72) : (!llvm.ptr) -> ()
    %73 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%73) : (!llvm.ptr) -> ()
    %74 = llvm.getelementptr %23[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<52 x i8>
    llvm.call @println(%74) : (!llvm.ptr) -> ()
    %75 = llvm.call @f_fe69c41f() : () -> i1
    llvm.store %75, %28 {alignment = 1 : i64} : i1, !llvm.ptr
    %76 = llvm.load %28 {alignment = 1 : i64} : !llvm.ptr -> i1
    llvm.cond_br %76, ^bb7, ^bb8
  ^bb7:  // pred: ^bb6
    %77 = llvm.getelementptr %25[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<59 x i8>
    llvm.call @println(%77) : (!llvm.ptr) -> ()
    llvm.br ^bb9
  ^bb8:  // pred: ^bb6
    %78 = llvm.getelementptr %24[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<52 x i8>
    llvm.call @println(%78) : (!llvm.ptr) -> ()
    llvm.br ^bb9
  ^bb9:  // 2 preds: ^bb7, ^bb8
    %79 = llvm.getelementptr %5[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<1 x i8>
    llvm.call @println(%79) : (!llvm.ptr) -> ()
    %80 = llvm.getelementptr %26[%2, %2] : (!llvm.ptr, i32, i32) -> !llvm.ptr, !llvm.array<44 x i8>
    llvm.call @println(%80) : (!llvm.ptr) -> ()
    llvm.call @jocky_sleep_and_recheck() : () -> ()
    llvm.return
  }
  llvm.func internal @f_53decde1() attributes {no_inline} {
    %0 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    %1 = llvm.mlir.constant(54 : i64) : i64
    %2 = llvm.call @f_61ae7c24(%0, %1) : (!llvm.ptr, i64) -> !llvm.ptr
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    %4 = llvm.mlir.constant(29 : i64) : i64
    %5 = llvm.call @f_61ae7c24(%3, %4) : (!llvm.ptr, i64) -> !llvm.ptr
    %6 = llvm.mlir.addressof @".str.2.enc" : !llvm.ptr
    %7 = llvm.mlir.constant(1 : i64) : i64
    %8 = llvm.call @f_61ae7c24(%6, %7) : (!llvm.ptr, i64) -> !llvm.ptr
    %9 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    %10 = llvm.mlir.constant(42 : i64) : i64
    %11 = llvm.call @f_61ae7c24(%9, %10) : (!llvm.ptr, i64) -> !llvm.ptr
    %12 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    %13 = llvm.mlir.constant(37 : i64) : i64
    %14 = llvm.call @f_61ae7c24(%12, %13) : (!llvm.ptr, i64) -> !llvm.ptr
    %15 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    %16 = llvm.mlir.constant(46 : i64) : i64
    %17 = llvm.call @f_61ae7c24(%15, %16) : (!llvm.ptr, i64) -> !llvm.ptr
    %18 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    %19 = llvm.mlir.constant(30 : i64) : i64
    %20 = llvm.call @f_61ae7c24(%18, %19) : (!llvm.ptr, i64) -> !llvm.ptr
    %21 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    %22 = llvm.mlir.constant(30 : i64) : i64
    %23 = llvm.call @f_61ae7c24(%21, %22) : (!llvm.ptr, i64) -> !llvm.ptr
    %24 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    %25 = llvm.mlir.constant(46 : i64) : i64
    %26 = llvm.call @f_61ae7c24(%24, %25) : (!llvm.ptr, i64) -> !llvm.ptr
    %27 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    %28 = llvm.mlir.constant(62 : i64) : i64
    %29 = llvm.call @f_61ae7c24(%27, %28) : (!llvm.ptr, i64) -> !llvm.ptr
    %30 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    %31 = llvm.mlir.constant(29 : i64) : i64
    %32 = llvm.call @f_61ae7c24(%30, %31) : (!llvm.ptr, i64) -> !llvm.ptr
    %33 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    %34 = llvm.mlir.constant(38 : i64) : i64
    %35 = llvm.call @f_61ae7c24(%33, %34) : (!llvm.ptr, i64) -> !llvm.ptr
    %36 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    %37 = llvm.mlir.constant(29 : i64) : i64
    %38 = llvm.call @f_61ae7c24(%36, %37) : (!llvm.ptr, i64) -> !llvm.ptr
    %39 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    %40 = llvm.mlir.constant(19 : i64) : i64
    %41 = llvm.call @f_61ae7c24(%39, %40) : (!llvm.ptr, i64) -> !llvm.ptr
    %42 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    %43 = llvm.mlir.constant(36 : i64) : i64
    %44 = llvm.call @f_61ae7c24(%42, %43) : (!llvm.ptr, i64) -> !llvm.ptr
    %45 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    %46 = llvm.mlir.constant(35 : i64) : i64
    %47 = llvm.call @f_61ae7c24(%45, %46) : (!llvm.ptr, i64) -> !llvm.ptr
    %48 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    %49 = llvm.mlir.constant(30 : i64) : i64
    %50 = llvm.call @f_61ae7c24(%48, %49) : (!llvm.ptr, i64) -> !llvm.ptr
    %51 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    %52 = llvm.mlir.constant(7 : i64) : i64
    %53 = llvm.call @f_61ae7c24(%51, %52) : (!llvm.ptr, i64) -> !llvm.ptr
    %54 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    %55 = llvm.mlir.constant(56 : i64) : i64
    %56 = llvm.call @f_61ae7c24(%54, %55) : (!llvm.ptr, i64) -> !llvm.ptr
    %57 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    %58 = llvm.mlir.constant(29 : i64) : i64
    %59 = llvm.call @f_61ae7c24(%57, %58) : (!llvm.ptr, i64) -> !llvm.ptr
    %60 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    %61 = llvm.mlir.constant(26 : i64) : i64
    %62 = llvm.call @f_61ae7c24(%60, %61) : (!llvm.ptr, i64) -> !llvm.ptr
    %63 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    %64 = llvm.mlir.constant(27 : i64) : i64
    %65 = llvm.call @f_61ae7c24(%63, %64) : (!llvm.ptr, i64) -> !llvm.ptr
    %66 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    %67 = llvm.mlir.constant(33 : i64) : i64
    %68 = llvm.call @f_61ae7c24(%66, %67) : (!llvm.ptr, i64) -> !llvm.ptr
    %69 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    %70 = llvm.mlir.constant(37 : i64) : i64
    %71 = llvm.call @f_61ae7c24(%69, %70) : (!llvm.ptr, i64) -> !llvm.ptr
    %72 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    %73 = llvm.mlir.constant(27 : i64) : i64
    %74 = llvm.call @f_61ae7c24(%72, %73) : (!llvm.ptr, i64) -> !llvm.ptr
    %75 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    %76 = llvm.mlir.constant(30 : i64) : i64
    %77 = llvm.call @f_61ae7c24(%75, %76) : (!llvm.ptr, i64) -> !llvm.ptr
    %78 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    %79 = llvm.mlir.constant(36 : i64) : i64
    %80 = llvm.call @f_61ae7c24(%78, %79) : (!llvm.ptr, i64) -> !llvm.ptr
    %81 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    %82 = llvm.mlir.constant(31 : i64) : i64
    %83 = llvm.call @f_61ae7c24(%81, %82) : (!llvm.ptr, i64) -> !llvm.ptr
    %84 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    %85 = llvm.mlir.constant(20 : i64) : i64
    %86 = llvm.call @f_61ae7c24(%84, %85) : (!llvm.ptr, i64) -> !llvm.ptr
    %87 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    %88 = llvm.mlir.constant(45 : i64) : i64
    %89 = llvm.call @f_61ae7c24(%87, %88) : (!llvm.ptr, i64) -> !llvm.ptr
    %90 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    %91 = llvm.mlir.constant(21 : i64) : i64
    %92 = llvm.call @f_61ae7c24(%90, %91) : (!llvm.ptr, i64) -> !llvm.ptr
    %93 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    %94 = llvm.mlir.constant(4 : i64) : i64
    %95 = llvm.call @f_61ae7c24(%93, %94) : (!llvm.ptr, i64) -> !llvm.ptr
    %96 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    %97 = llvm.mlir.constant(2 : i64) : i64
    %98 = llvm.call @f_61ae7c24(%96, %97) : (!llvm.ptr, i64) -> !llvm.ptr
    %99 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    %100 = llvm.mlir.constant(3 : i64) : i64
    %101 = llvm.call @f_61ae7c24(%99, %100) : (!llvm.ptr, i64) -> !llvm.ptr
    %102 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    %103 = llvm.mlir.constant(28 : i64) : i64
    %104 = llvm.call @f_61ae7c24(%102, %103) : (!llvm.ptr, i64) -> !llvm.ptr
    %105 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    %106 = llvm.mlir.constant(19 : i64) : i64
    %107 = llvm.call @f_61ae7c24(%105, %106) : (!llvm.ptr, i64) -> !llvm.ptr
    %108 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    %109 = llvm.mlir.constant(52 : i64) : i64
    %110 = llvm.call @f_61ae7c24(%108, %109) : (!llvm.ptr, i64) -> !llvm.ptr
    %111 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    %112 = llvm.mlir.constant(34 : i64) : i64
    %113 = llvm.call @f_61ae7c24(%111, %112) : (!llvm.ptr, i64) -> !llvm.ptr
    %114 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    %115 = llvm.mlir.constant(50 : i64) : i64
    %116 = llvm.call @f_61ae7c24(%114, %115) : (!llvm.ptr, i64) -> !llvm.ptr
    %117 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    %118 = llvm.mlir.constant(45 : i64) : i64
    %119 = llvm.call @f_61ae7c24(%117, %118) : (!llvm.ptr, i64) -> !llvm.ptr
    %120 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    %121 = llvm.mlir.constant(45 : i64) : i64
    %122 = llvm.call @f_61ae7c24(%120, %121) : (!llvm.ptr, i64) -> !llvm.ptr
    %123 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    %124 = llvm.mlir.constant(53 : i64) : i64
    %125 = llvm.call @f_61ae7c24(%123, %124) : (!llvm.ptr, i64) -> !llvm.ptr
    %126 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    %127 = llvm.mlir.constant(11 : i64) : i64
    %128 = llvm.call @f_61ae7c24(%126, %127) : (!llvm.ptr, i64) -> !llvm.ptr
    %129 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    %130 = llvm.mlir.constant(27 : i64) : i64
    %131 = llvm.call @f_61ae7c24(%129, %130) : (!llvm.ptr, i64) -> !llvm.ptr
    %132 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    %133 = llvm.mlir.constant(47 : i64) : i64
    %134 = llvm.call @f_61ae7c24(%132, %133) : (!llvm.ptr, i64) -> !llvm.ptr
    %135 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    %136 = llvm.mlir.constant(57 : i64) : i64
    %137 = llvm.call @f_61ae7c24(%135, %136) : (!llvm.ptr, i64) -> !llvm.ptr
    %138 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    %139 = llvm.mlir.constant(58 : i64) : i64
    %140 = llvm.call @f_61ae7c24(%138, %139) : (!llvm.ptr, i64) -> !llvm.ptr
    %141 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    %142 = llvm.mlir.constant(27 : i64) : i64
    %143 = llvm.call @f_61ae7c24(%141, %142) : (!llvm.ptr, i64) -> !llvm.ptr
    %144 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    %145 = llvm.mlir.constant(19 : i64) : i64
    %146 = llvm.call @f_61ae7c24(%144, %145) : (!llvm.ptr, i64) -> !llvm.ptr
    %147 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    %148 = llvm.mlir.constant(17 : i64) : i64
    %149 = llvm.call @f_61ae7c24(%147, %148) : (!llvm.ptr, i64) -> !llvm.ptr
    %150 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    %151 = llvm.mlir.constant(12 : i64) : i64
    %152 = llvm.call @f_61ae7c24(%150, %151) : (!llvm.ptr, i64) -> !llvm.ptr
    %153 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    %154 = llvm.mlir.constant(44 : i64) : i64
    %155 = llvm.call @f_61ae7c24(%153, %154) : (!llvm.ptr, i64) -> !llvm.ptr
    %156 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    %157 = llvm.mlir.constant(54 : i64) : i64
    %158 = llvm.call @f_61ae7c24(%156, %157) : (!llvm.ptr, i64) -> !llvm.ptr
    %159 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    %160 = llvm.mlir.constant(47 : i64) : i64
    %161 = llvm.call @f_61ae7c24(%159, %160) : (!llvm.ptr, i64) -> !llvm.ptr
    %162 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    %163 = llvm.mlir.constant(34 : i64) : i64
    %164 = llvm.call @f_61ae7c24(%162, %163) : (!llvm.ptr, i64) -> !llvm.ptr
    %165 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    %166 = llvm.mlir.constant(53 : i64) : i64
    %167 = llvm.call @f_61ae7c24(%165, %166) : (!llvm.ptr, i64) -> !llvm.ptr
    %168 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    %169 = llvm.mlir.constant(47 : i64) : i64
    %170 = llvm.call @f_61ae7c24(%168, %169) : (!llvm.ptr, i64) -> !llvm.ptr
    %171 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    %172 = llvm.mlir.constant(38 : i64) : i64
    %173 = llvm.call @f_61ae7c24(%171, %172) : (!llvm.ptr, i64) -> !llvm.ptr
    %174 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    %175 = llvm.mlir.constant(28 : i64) : i64
    %176 = llvm.call @f_61ae7c24(%174, %175) : (!llvm.ptr, i64) -> !llvm.ptr
    %177 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    %178 = llvm.mlir.constant(17 : i64) : i64
    %179 = llvm.call @f_61ae7c24(%177, %178) : (!llvm.ptr, i64) -> !llvm.ptr
    %180 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    %181 = llvm.mlir.constant(52 : i64) : i64
    %182 = llvm.call @f_61ae7c24(%180, %181) : (!llvm.ptr, i64) -> !llvm.ptr
    %183 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    %184 = llvm.mlir.constant(59 : i64) : i64
    %185 = llvm.call @f_61ae7c24(%183, %184) : (!llvm.ptr, i64) -> !llvm.ptr
    %186 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    %187 = llvm.mlir.constant(52 : i64) : i64
    %188 = llvm.call @f_61ae7c24(%186, %187) : (!llvm.ptr, i64) -> !llvm.ptr
    %189 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    %190 = llvm.mlir.constant(44 : i64) : i64
    %191 = llvm.call @f_61ae7c24(%189, %190) : (!llvm.ptr, i64) -> !llvm.ptr
    llvm.return
  }
  llvm.mlir.global_ctors ctors = [@f_53decde1, @f_64c2918f], priorities = [101 : i32, 101 : i32], data = [#llvm.zero, #llvm.zero]
  llvm.func internal @f_4b0d211b(%arg0: !llvm.ptr, %arg1: i32) attributes {no_inline} {
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
  llvm.func internal @f_64c2918f() attributes {no_inline} {
    %0 = llvm.mlir.constant(54 : i32) : i32
    %1 = llvm.mlir.addressof @".str.0.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%1, %0) : (!llvm.ptr, i32) -> ()
    %2 = llvm.mlir.constant(29 : i32) : i32
    %3 = llvm.mlir.addressof @".str.1.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%3, %2) : (!llvm.ptr, i32) -> ()
    %4 = llvm.mlir.constant(42 : i32) : i32
    %5 = llvm.mlir.addressof @".str.3.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%5, %4) : (!llvm.ptr, i32) -> ()
    %6 = llvm.mlir.constant(37 : i32) : i32
    %7 = llvm.mlir.addressof @".str.4.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%7, %6) : (!llvm.ptr, i32) -> ()
    %8 = llvm.mlir.constant(46 : i32) : i32
    %9 = llvm.mlir.addressof @".str.5.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%9, %8) : (!llvm.ptr, i32) -> ()
    %10 = llvm.mlir.constant(30 : i32) : i32
    %11 = llvm.mlir.addressof @".str.6.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%11, %10) : (!llvm.ptr, i32) -> ()
    %12 = llvm.mlir.constant(30 : i32) : i32
    %13 = llvm.mlir.addressof @".str.7.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%13, %12) : (!llvm.ptr, i32) -> ()
    %14 = llvm.mlir.constant(46 : i32) : i32
    %15 = llvm.mlir.addressof @".str.8.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%15, %14) : (!llvm.ptr, i32) -> ()
    %16 = llvm.mlir.constant(62 : i32) : i32
    %17 = llvm.mlir.addressof @".str.9.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%17, %16) : (!llvm.ptr, i32) -> ()
    %18 = llvm.mlir.constant(29 : i32) : i32
    %19 = llvm.mlir.addressof @".str.10.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%19, %18) : (!llvm.ptr, i32) -> ()
    %20 = llvm.mlir.constant(38 : i32) : i32
    %21 = llvm.mlir.addressof @".str.11.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%21, %20) : (!llvm.ptr, i32) -> ()
    %22 = llvm.mlir.constant(29 : i32) : i32
    %23 = llvm.mlir.addressof @".str.12.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%23, %22) : (!llvm.ptr, i32) -> ()
    %24 = llvm.mlir.constant(19 : i32) : i32
    %25 = llvm.mlir.addressof @".str.13.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%25, %24) : (!llvm.ptr, i32) -> ()
    %26 = llvm.mlir.constant(36 : i32) : i32
    %27 = llvm.mlir.addressof @".str.14.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%27, %26) : (!llvm.ptr, i32) -> ()
    %28 = llvm.mlir.constant(35 : i32) : i32
    %29 = llvm.mlir.addressof @".str.15.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%29, %28) : (!llvm.ptr, i32) -> ()
    %30 = llvm.mlir.constant(30 : i32) : i32
    %31 = llvm.mlir.addressof @".str.16.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%31, %30) : (!llvm.ptr, i32) -> ()
    %32 = llvm.mlir.constant(7 : i32) : i32
    %33 = llvm.mlir.addressof @".str.17.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%33, %32) : (!llvm.ptr, i32) -> ()
    %34 = llvm.mlir.constant(56 : i32) : i32
    %35 = llvm.mlir.addressof @".str.18.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%35, %34) : (!llvm.ptr, i32) -> ()
    %36 = llvm.mlir.constant(29 : i32) : i32
    %37 = llvm.mlir.addressof @".str.19.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%37, %36) : (!llvm.ptr, i32) -> ()
    %38 = llvm.mlir.constant(26 : i32) : i32
    %39 = llvm.mlir.addressof @".str.20.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%39, %38) : (!llvm.ptr, i32) -> ()
    %40 = llvm.mlir.constant(27 : i32) : i32
    %41 = llvm.mlir.addressof @".str.21.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%41, %40) : (!llvm.ptr, i32) -> ()
    %42 = llvm.mlir.constant(33 : i32) : i32
    %43 = llvm.mlir.addressof @".str.22.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%43, %42) : (!llvm.ptr, i32) -> ()
    %44 = llvm.mlir.constant(37 : i32) : i32
    %45 = llvm.mlir.addressof @".str.23.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%45, %44) : (!llvm.ptr, i32) -> ()
    %46 = llvm.mlir.constant(27 : i32) : i32
    %47 = llvm.mlir.addressof @".str.24.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%47, %46) : (!llvm.ptr, i32) -> ()
    %48 = llvm.mlir.constant(30 : i32) : i32
    %49 = llvm.mlir.addressof @".str.25.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%49, %48) : (!llvm.ptr, i32) -> ()
    %50 = llvm.mlir.constant(36 : i32) : i32
    %51 = llvm.mlir.addressof @".str.26.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%51, %50) : (!llvm.ptr, i32) -> ()
    %52 = llvm.mlir.constant(31 : i32) : i32
    %53 = llvm.mlir.addressof @".str.27.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%53, %52) : (!llvm.ptr, i32) -> ()
    %54 = llvm.mlir.constant(20 : i32) : i32
    %55 = llvm.mlir.addressof @".str.28.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%55, %54) : (!llvm.ptr, i32) -> ()
    %56 = llvm.mlir.constant(45 : i32) : i32
    %57 = llvm.mlir.addressof @".str.29.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%57, %56) : (!llvm.ptr, i32) -> ()
    %58 = llvm.mlir.constant(21 : i32) : i32
    %59 = llvm.mlir.addressof @".str.30.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%59, %58) : (!llvm.ptr, i32) -> ()
    %60 = llvm.mlir.constant(4 : i32) : i32
    %61 = llvm.mlir.addressof @".str.31.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%61, %60) : (!llvm.ptr, i32) -> ()
    %62 = llvm.mlir.constant(2 : i32) : i32
    %63 = llvm.mlir.addressof @".str.32.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%63, %62) : (!llvm.ptr, i32) -> ()
    %64 = llvm.mlir.constant(3 : i32) : i32
    %65 = llvm.mlir.addressof @".str.33.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%65, %64) : (!llvm.ptr, i32) -> ()
    %66 = llvm.mlir.constant(28 : i32) : i32
    %67 = llvm.mlir.addressof @".str.34.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%67, %66) : (!llvm.ptr, i32) -> ()
    %68 = llvm.mlir.constant(19 : i32) : i32
    %69 = llvm.mlir.addressof @".str.35.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%69, %68) : (!llvm.ptr, i32) -> ()
    %70 = llvm.mlir.constant(52 : i32) : i32
    %71 = llvm.mlir.addressof @".str.36.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%71, %70) : (!llvm.ptr, i32) -> ()
    %72 = llvm.mlir.constant(34 : i32) : i32
    %73 = llvm.mlir.addressof @".str.37.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%73, %72) : (!llvm.ptr, i32) -> ()
    %74 = llvm.mlir.constant(50 : i32) : i32
    %75 = llvm.mlir.addressof @".str.38.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%75, %74) : (!llvm.ptr, i32) -> ()
    %76 = llvm.mlir.constant(45 : i32) : i32
    %77 = llvm.mlir.addressof @".str.39.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%77, %76) : (!llvm.ptr, i32) -> ()
    %78 = llvm.mlir.constant(45 : i32) : i32
    %79 = llvm.mlir.addressof @".str.40.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%79, %78) : (!llvm.ptr, i32) -> ()
    %80 = llvm.mlir.constant(53 : i32) : i32
    %81 = llvm.mlir.addressof @".str.41.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%81, %80) : (!llvm.ptr, i32) -> ()
    %82 = llvm.mlir.constant(11 : i32) : i32
    %83 = llvm.mlir.addressof @".str.42.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%83, %82) : (!llvm.ptr, i32) -> ()
    %84 = llvm.mlir.constant(27 : i32) : i32
    %85 = llvm.mlir.addressof @".str.43.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%85, %84) : (!llvm.ptr, i32) -> ()
    %86 = llvm.mlir.constant(47 : i32) : i32
    %87 = llvm.mlir.addressof @".str.44.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%87, %86) : (!llvm.ptr, i32) -> ()
    %88 = llvm.mlir.constant(57 : i32) : i32
    %89 = llvm.mlir.addressof @".str.45.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%89, %88) : (!llvm.ptr, i32) -> ()
    %90 = llvm.mlir.constant(58 : i32) : i32
    %91 = llvm.mlir.addressof @".str.46.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%91, %90) : (!llvm.ptr, i32) -> ()
    %92 = llvm.mlir.constant(27 : i32) : i32
    %93 = llvm.mlir.addressof @".str.47.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%93, %92) : (!llvm.ptr, i32) -> ()
    %94 = llvm.mlir.constant(19 : i32) : i32
    %95 = llvm.mlir.addressof @".str.48.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%95, %94) : (!llvm.ptr, i32) -> ()
    %96 = llvm.mlir.constant(17 : i32) : i32
    %97 = llvm.mlir.addressof @".str.49.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%97, %96) : (!llvm.ptr, i32) -> ()
    %98 = llvm.mlir.constant(12 : i32) : i32
    %99 = llvm.mlir.addressof @".str.50.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%99, %98) : (!llvm.ptr, i32) -> ()
    %100 = llvm.mlir.constant(44 : i32) : i32
    %101 = llvm.mlir.addressof @".str.51.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%101, %100) : (!llvm.ptr, i32) -> ()
    %102 = llvm.mlir.constant(54 : i32) : i32
    %103 = llvm.mlir.addressof @".str.52.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%103, %102) : (!llvm.ptr, i32) -> ()
    %104 = llvm.mlir.constant(47 : i32) : i32
    %105 = llvm.mlir.addressof @".str.53.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%105, %104) : (!llvm.ptr, i32) -> ()
    %106 = llvm.mlir.constant(34 : i32) : i32
    %107 = llvm.mlir.addressof @".str.54.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%107, %106) : (!llvm.ptr, i32) -> ()
    %108 = llvm.mlir.constant(53 : i32) : i32
    %109 = llvm.mlir.addressof @".str.55.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%109, %108) : (!llvm.ptr, i32) -> ()
    %110 = llvm.mlir.constant(47 : i32) : i32
    %111 = llvm.mlir.addressof @".str.56.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%111, %110) : (!llvm.ptr, i32) -> ()
    %112 = llvm.mlir.constant(38 : i32) : i32
    %113 = llvm.mlir.addressof @".str.57.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%113, %112) : (!llvm.ptr, i32) -> ()
    %114 = llvm.mlir.constant(28 : i32) : i32
    %115 = llvm.mlir.addressof @".str.58.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%115, %114) : (!llvm.ptr, i32) -> ()
    %116 = llvm.mlir.constant(17 : i32) : i32
    %117 = llvm.mlir.addressof @".str.59.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%117, %116) : (!llvm.ptr, i32) -> ()
    %118 = llvm.mlir.constant(52 : i32) : i32
    %119 = llvm.mlir.addressof @".str.60.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%119, %118) : (!llvm.ptr, i32) -> ()
    %120 = llvm.mlir.constant(59 : i32) : i32
    %121 = llvm.mlir.addressof @".str.61.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%121, %120) : (!llvm.ptr, i32) -> ()
    %122 = llvm.mlir.constant(52 : i32) : i32
    %123 = llvm.mlir.addressof @".str.62.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%123, %122) : (!llvm.ptr, i32) -> ()
    %124 = llvm.mlir.constant(44 : i32) : i32
    %125 = llvm.mlir.addressof @".str.63.enc" : !llvm.ptr
    llvm.call @f_4b0d211b(%125, %124) : (!llvm.ptr, i32) -> ()
    llvm.return
  }
}

