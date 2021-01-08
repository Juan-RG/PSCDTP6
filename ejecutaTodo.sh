#!/usr/bin/env bash
./Servidores/ServidorRegistro localhost localhost localhost 2021 2022 2023 2020&

./Servidores/ServidorLinda 2021&
./Servidores/ServidorLinda 2022&
./Servidores/ServidorLinda 2023&

./Clientes/InitTuplas localhost 2020&
./Clientes/Publicador localhost 2020&
./Clientes/Buscador localhost 2020&
./Clientes/BuscadorCombinado localhost 2020&
