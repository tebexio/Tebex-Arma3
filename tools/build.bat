:: Normalize our location
cd cd /d %~dp0

rmdir ../build /S /Q
mkdir ../build

mkdir ../build
cd ../build


mkdir build32
mkdir build64

cd build32
cmake ../.. -G "Visual Studio 14 2015"
:: make
mv ../libTebex_Arma3.dll ../libTebex_Arma3.32.dll

cd ../build64
cmake ../.. -G "Visual Studio 14 2015 Win64"
:: make
mv ../libTebex_Arma3.dll ../libTebex_Arma3.64.dll