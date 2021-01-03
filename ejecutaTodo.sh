#!/usr/bin/env bash
./MainServerDespliegue localhost localhost localhost 2021 2022 2023 2020&

./ServidorLinda 2021&
./ServidorLinda 2022&
./ServidorLinda 2023&

./InitTuplas localhost 2020&
./MainPublicador localhost 2020&
./MainBuscador localhost 2020&
./MainBuscadorCombinado localhost 2020&
