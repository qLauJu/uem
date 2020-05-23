module.exports = app => {
    const domain = app.domain.calculos;

    const Pedido = app.db.models.Pedido;
    const Cacamba = app.db.models.Cacamba;
    const CacambaPreco = app.db.models.CacambaPreco;
    const Usuario = app.db.models.Usuario;
    const Config = app.db.models.Config;

    return {
        listarPedidosGerente: (req, res) => {
            Pedido.findAll({
                include: { model: CacambaPreco, include: [Cacamba] }
            })
                .then(pedidos => {
                    console.log(pedidos);
                    res.json(pedidos.map(pedido => {
                        return {
                            id: pedido.id,
                            cacambaId: pedido.CacambaPrecoId,
                            endereco: pedido.endereco,
                            tamanhoCacamba: pedido.CacambaPreco.Cacamba.tamanho,
                            descricaoCacamba: pedido.CacambaPreco.descricao,
                            qtdeCacambas: pedido.qtdeCacambas,
                            dataEntrega: pedido.dataEntrega,
                            qtdeDias: pedido.qtdeDias,
                            dataDevolucao: pedido.dataDevolucao,
                            clienteDocumento: pedido.clienteDocumento,
                            valorTotal: pedido.valorTotal,
                            status: pedido.status,
                            observacoes: pedido.observacoes
                        };
                    }));
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        listarPedidosCliente: (req, res) => {
            Pedido.findAll({
                where: {
                    clienteDocumento: req.user.documento,
                },
                include: {
                    model: CacambaPreco,
                    include: [Cacamba]
                }
            })
                .then(pedidos => {
                    res.json(pedidos.map(pedido => {
                        return {
                            id: pedido.id,
                            endereco: pedido.endereco,
                            cacambaId: pedido.CacambaPrecoId,
                            tamanhoCacamba: pedido.CacambaPreco.Cacamba.tamanho,
                            descricaoCacamba: pedido.CacambaPreco.descricao,
                            qtdeCacambas: pedido.qtdeCacambas,
                            dataEntrega: pedido.dataEntrega,
                            qtdeDias: pedido.qtdeDias,
                            dataDevolucao: pedido.dataDevolucao,
                            clienteDocumento: pedido.clienteDocumento,
                            valorTotal: pedido.valorTotal,
                            status: pedido.status,
                            observacoes: pedido.observacoes
                        };
                    }));
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        criarPedidoGerente: (req, res) => {
            if (req.body.clienteDocumento == undefined) {
                res.status(412).json({
                    mensages: "Por favor, informe o cliente.",
                    sucesso: false
                });
            } else {
                Config.findById(req.user.id)
                    .then(config => {
                        CacambaPreco.findById(req.body.CacambaPrecoId)
                            .then(cacamba => {
                                req.body.valorTotal = domain.calcularValorPedido(cacamba.valor, req.body.qtdeCacambas, req.body.qtdeDias, config.qtdePadraoDeDias);
                                let dataEntrega = new Date(req.body.dataEntrega);
                                req.body.dataDevolucao = domain.calcularDataDevolucao(dataEntrega, req.body.qtdeDias);
                                Pedido.create(req.body)
                                    .then(pedido => {
                                        // TODO: Decrementar quantidade de caçambas.
                                        CacambaPreco.findById(pedido.CacambaPrecoId, {
                                            include: [Cacamba]
                                        })
                                            .then(cacamba => {
                                                res.json({
                                                    id: pedido.id,
                                                    endereco: pedido.endereco,
                                                    cacambaId: pedido.CacambaPrecoId,
                                                    tamanhoCacamba: cacamba.Cacamba.tamanho,
                                                    descricaoCacamba: cacamba.descricao,
                                                    qtdeCacambas: pedido.qtdeCacambas,
                                                    dataEntrega: pedido.dataEntrega,
                                                    qtdeDias: pedido.qtdeDias,
                                                    dataDevolucao: pedido.dataDevolucao,
                                                    clienteDocumento: pedido.clienteDocumento,
                                                    valorTotal: pedido.valorTotal,
                                                    status: pedido.status,
                                                    observacoes: pedido.observacoes
                                                })
                                            })
                                    })
                                    .catch(error => {
                                        res.status(412).json({
                                            mensagem: error.message,
                                            sucesso: false
                                        });
                                    });
                            }).catch(error => {
                                res.status(412).json({
                                    mensagem: error.message,
                                    sucesso: false
                                });
                            });
                    })
                    .catch(error => {
                        res.status(412).json({
                            mensagem: "Erro ao encontrar as configurações do usuário.",
                            sucesso: false
                        });
                    });
            }
        },

        criarPedidoCliente: (req, res) => {
            req.body.clienteDocumento = req.user.documento;
            Config.findById(req.user.id)
                .then(config => {
                    CacambaPreco.findById(req.body.CacambaPrecoId)
                        .then(cacamba => {
                            req.body.valorTotal = domain.calcularValorPedido(cacamba.valor, req.body.qtdeCacambas, req.body.qtdeDias, config.qtdePadraoDeDias);
                            let dataEntrega = new Date(req.body.dataEntrega);
                            req.body.dataDevolucao = domain.calcularDataDevolucao(dataEntrega, req.body.qtdeDias);
                            Pedido.create(req.body)
                                .then(pedido => {
                                    // TODO: Decrementar quantidade de caçambas.
                                    CacambaPreco.findById(pedido.CacambaPrecoId, {
                                        include: [Cacamba]
                                    })
                                        .then(cacamba => {
                                            res.json({
                                                id: pedido.id,
                                                cacambaId: pedido.CacambaPrecoId,
                                                endereco: pedido.endereco,
                                                tamanhoCacamba: cacamba.Cacamba.tamanho,
                                                descricaoCacamba: cacamba.descricao,
                                                qtdeCacambas: pedido.qtdeCacambas,
                                                dataEntrega: pedido.dataEntrega,
                                                qtdeDias: pedido.qtdeDias,
                                                dataDevolucao: pedido.dataDevolucao,
                                                clienteDocumento: pedido.clienteDocumento,
                                                valorTotal: pedido.valorTotal,
                                                status: pedido.status,
                                                observacoes: pedido.observacoes
                                            })
                                        })
                                })
                                .catch(error => {
                                    res.status(412).json({
                                        mensagem: error.message,
                                        sucesso: false
                                    });
                                });
                        }).catch(error => {
                            res.status(412).json({
                                mensagem: error.message,
                                sucesso: false
                            });
                        });
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: "Erro ao encontrar as configurações do usuário.",
                        sucesso: false
                    });
                });
        },

        calcularPedido: (req, res) => {
            Config.findById(req.user.id)
                .then(config => {
                    console.log(config);
                    CacambaPreco.findById(req.body.CacambaPrecoId)
                        .then(cacamba => {
                            let valorTotal = domain.calcularValorPedido(cacamba.valor, req.body.qtdeCacambas, req.body.qtdeDias, config.qtdePadraoDeDias);
                            let dataEntrega = new Date(req.body.dataEntrega);
                            let dataDevolucao = domain.calcularDataDevolucao(dataEntrega, req.body.qtdeDias);
                            res.json({
                                valor: valorTotal,
                                dataDevolucao: dataDevolucao
                            });
                        }).catch(error => {
                            res.status(412).json({
                                mensagem: error.message,
                                sucesso: false
                            });
                        });
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: "Erro ao encontrar as configurações do usuário.",
                        sucesso: false
                    });
                });
        },

        alterarPedido: (req, res) => {
            req.body.id = req.params.id;
            Config.findById(req.user.id)
                .then(config => {
                    CacambaPreco.findById(req.body.cacambaId)
                        .then(cacamba => {
                            console.log(cacamba);
                            let dataEntrega = new Date(req.body.dataEntrega);
                            let dataDevolucao = domain.calcularDataDevolucao(dataEntrega, req.body.qtdeDias);
                            req.body.valorTotal = domain.calcularValorPedido(cacamba.valor, req.body.qtdeCacambas, req.body.qtdeDias, config.qtdePadraoDeDias);
                            req.body.dataDevolucao = dataDevolucao;
                            Pedido.upsert(req.body, { fields: Object.keys(req.body) })
                                .then(response => res.json({ mensagem: "Pedido alterado com sucesso." }))
                                .catch(error => {
                                    res.status(412).json({
                                        mensagem: error.message,
                                        sucesso: false
                                    });
                                });
                        }).catch(error => {
                            res.status(412).json({
                                mensagem: "Erro ao encontrar a caçamba.",
                                sucesso: false
                            });
                        });
                })
                .catch(error => {
                    res.status(412).send({
                        mensagem: 'Erro ao encontrar as configurações do usuário',
                        sucess: false
                    })
                })
        },

        removerPedido: (req, res) => {
            Pedido.findById(req.params.id)
                .then(pedido => pedido.destroy({ force: true }))
                .then(() => res.json({ mensagem: "Pedido removido com sucesso." }))
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        listarStatus: (req, res) => {
            res.json(["Aguardando entrega", "Entregue", "Aguardando pagamento", 'Concluído']);
        }
    };
}