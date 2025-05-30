set -x

pushd .
cd ClientApp
yarn run build
popd
g++ -O2 main.cpp -I./Crow/include/ -I./asio/asio/include
