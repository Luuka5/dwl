{
  lib,
  stdenv,
  installShellFiles,
  libX11,
  libinput,
  libxcb,
  libxkbcommon,
  pixman,
  pkg-config,
  wayland-scanner,
  wayland,
  wayland-protocols,
  wlroots_0_18,
  xcbutilwm,
  xwayland,
  gnumake,
  fcft,
  swaylock,
  kanshi,
  pkgs
}:

let
  makeScript = name: runtimeInputs: pkgs.writeShellApplication {
    inherit name;
    runtimeInputs = runtimeInputs;
    text = builtins.readFile ./scripts/${name}.sh;
  };

  dwl = stdenv.mkDerivation {
    pname = "dwl";
    version = "0.7";

    src = ./dwl;

    nativeBuildInputs = [
      installShellFiles
      pkg-config
      gnumake
    ];

    buildInputs = [
      libinput
      libxcb
      libxkbcommon
      pixman
      wayland
      wayland-protocols
      wlroots_0_18
      libX11
      xcbutilwm
      xwayland
      wayland-scanner
    ];

    outputs = [ "out" "man" ];

    makeFlags = [
      "PKG_CONFIG=${stdenv.cc.targetPrefix}pkg-config"
      "WAYLAND_SCANNER=wayland-scanner"
      "PREFIX=$(out)"
      "MANDIR=$(man)/share/man"
    ];

    buildPhase = ''
      make clean
      make
    '';

    meta = {
      homepage = "https://github.com/tomaskallup/dwl/";
      description = "Dynamic window manager for Wayland";
      longDescription = ''
        dwl is a compact, hackable compositor for Wayland based on wlroots. It is
        intended to fill the same space in the Wayland world that dwm does in X11,
        primarily in terms of philosophy, and secondarily in terms of
        functionality. Like dwm, dwl is:

        - Easy to understand, hack on, and extend with patches
        - One C source file (or a very small number) configurable via config.h
        - Limited to 2000 SLOC to promote hackability
        - Tied to as few external dependencies as possible
      '';
      license = lib.licenses.gpl3Only;
      maintainers = [ lib.maintainers.AndersonTorres ];
      inherit (wayland.meta) platforms;
      mainProgram = "dwl";
    };
  };

  dwlb = stdenv.mkDerivation {
    pname = "dwlb";
    version = "0.1";

    src = ./dwlb;

    nativeBuildInputs = [
      installShellFiles
      pkg-config
      gnumake
    ];

    buildInputs = [
      fcft 
      libinput
      libxcb
      libxkbcommon
      pixman
      wayland
      wayland-protocols
      wlroots_0_18
      libX11
      xcbutilwm
      xwayland
      wayland-scanner
    ];

    outputs = [ "out" "man" ];

    makeFlags = [
      "PKG_CONFIG=${stdenv.cc.targetPrefix}pkg-config"
      "WAYLAND_SCANNER=wayland-scanner"
      "PREFIX=$(out)"
      "MANDIR=$(man)/share/man"
    ];

    buildPhase = ''
      make clean
      make
    '';

    meta = {
      homepage = "https://github.com/kolumni/dwlb/";
      description = "A fast, feature-complete bar for dwl."; 
      longDescription = ''
        A fast, feature-complete bar for dwl."; 
      '';
      inherit (wayland.meta) platforms;
      mainProgram = "dwlb";
    };
  };
in

pkgs.symlinkJoin {
  name = "dwl-custom";
  paths = [
    dwl
    dwlb
    (makeScript "bt-last-device" [ pkgs.blueman ])

    (makeScript "clipscreenshot" [ pkgs.grim pkgs.slurp ])
    (makeScript "savescreenshot" [ pkgs.grim pkgs.slurp ])
    (makeScript "screenshot" [ pkgs.grim pkgs.slurp ])

    (makeScript "track" [ ]) 
    (makeScript "status" [ pkgs.iproute2 ]) 
    (makeScript "run-status" [ ]) 

    (makeScript "media-control" [ ]) 
    (makeScript "brightness-control" [ ])
    
    (makeScript "lock" [ swaylock ]) # ?
    (makeScript "locksuspend" [ pkgs.kanshi ]) # ?

    (makeScript "start-wm" [ dwl dwlb]) # ?
    (makeScript "login-screen" [ dwl dwlb pkgs.swaylock ]) # ?
    (makeScript "post-startup" [ pkgs.kanshi pkgs.swayidle ]) # ?
  ];
}
