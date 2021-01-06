#!/usr/bin/env bash

./Clientes/InitTuplas localhost 2020&
./Clientes/MainPublicador localhost 2020&
./Clientes/MainBuscador localhost 2020&
./Clientes/MainBuscadorCombinado localhost 2020&
