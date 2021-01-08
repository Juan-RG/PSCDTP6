#!/usr/bin/env bash

./Clientes/InitTuplas localhost 2020&
./Clientes/Publicador localhost 2020&
./Clientes/Buscador localhost 2020&
./Clientes/BuscadorCombinado localhost 2020&
