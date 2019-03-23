#!/usr/bin/env bash
rm -rf hereup
mkdir hereup
eosio-cpp -abigen -o hereup/hereup.wasm hereup.cpp
