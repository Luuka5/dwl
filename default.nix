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
  makeWrapper,
  pkgs
}:

let
  home-utilities = pkgs.callPackage (pkgs.fetchFromGitHub {
    owner = "Luuka5";
    repo = "home-utilities";
    rev = "59191c08e7c390e0ef0d7e2591e0afd5f3567a4c";
    hash = "sha256-ef1w9V3Zc4AVPVC49rqIibe9h7ObP3l/CL8cMoRek8s=";
  }) {};

in

stdenv.mkDerivation ({
  pname = "dwl";
  version = "0.7";

  src = builtins.path { name = "dwl-custom"; path = ./.; };

  nativeBuildInputs = [
    installShellFiles
    pkg-config
    gnumake
    makeWrapper
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
    home-utilities
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

  postInstall = ''
    wrapProgram $out/bin/dwl \
      --prefix PATH : ${home-utilities}/bin
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

  passthru = {
    inherit home-utilities;
  };

})
