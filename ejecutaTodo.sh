#!/usr/bin/env bash
./Servidores/MainServerDespliegue localhost localhost localhost 2021 2022 2023 2020&

./Servidores/ServidorLinda 2021&
./Servidores/ServidorLinda 2022&
./Servidores/ServidorLinda 2023&

./Clientes/InitTuplas localhost 2020&
./Clientes/MainPublicador localhost 2020&
./Clientes/MainBuscador localhost 2020&
./Clientes/MainBuscadorCombinado localhost 2020&
