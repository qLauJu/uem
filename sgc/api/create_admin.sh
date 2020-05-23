#!/bin/bash

curl -H "Content-Type: application/json" -X POST -d '{"tipo":"GERENTE", "documento": "admin", "senha":"admin"}' http://localhost:3000/usuarios
