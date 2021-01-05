#!/usr/bin/env bash
./MainServerDespliegue localhost localhost localhost 2021 2022 2023 2020&

./ServidorLinda 2021&
./ServidorLinda 2022&
./ServidorLinda 2023&
