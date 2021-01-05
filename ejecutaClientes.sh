#!/usr/bin/env bash

./InitTuplas localhost 2020&
./MainPublicador localhost 2020&
./MainBuscador localhost 2020&
./MainBuscadorCombinado localhost 2020&
