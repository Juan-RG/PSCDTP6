#!/usr/bin/env bash

./Clientes/InitTuplas localhost 2020&
sleep 1

./Clientes/Publicador localhost 2020&
sleep 1

./Clientes/Buscador localhost 2020&
sleep 1

./Clientes/BuscadorCombinado localhost 2020&
