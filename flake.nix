{
  description = "Engine";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, ... } : let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in {

    packages.x86_64-linux.engine = pkgs.stdenv.mkDerivation {
      name = "engine";
      src = ./.;
      buildInputs = with pkgs; [ glfw ];
      nativeBuildInputs = with pkgs; [ cmake ];

      installPhase = ''
      mkdir -p $out/bin
      install -Dm755 engine $out/bin
      '';

    };

    packages.x86_64-linux.default = self.packages.x86_64-linux.engine;
  };
}
