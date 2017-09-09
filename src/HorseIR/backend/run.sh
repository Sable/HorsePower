#!/bin/bash
echo Starting ...
(cd ../ && make release && ./horse -t 6)
echo Done.
