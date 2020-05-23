module.exports = app => {
    const CacambaPreco = app.db.models.CacambaPreco;
    const Cacamba = app.db.models.Cacamba;
    const Pedidos = app.db.models.Pedido;

    return {
        getAll: (req, res) => {
            CacambaPreco.findAll({
                    include: [Cacamba]
                })
                .then(cacambas => {
                    const response = cacambas.map(cacamba => {
                        return {
                            id: cacamba.id,
                            descricao: cacamba.descricao,
                            tamanho: cacamba.Cacamba.tamanho,
                            quantidade: cacamba.Cacamba.quantidade,
                            valor: cacamba.valor
                        }
                    });
                    res.json(response);
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        criarCacamba: (req, res) => {
            Cacamba.findOne({ where: { tamanho: req.body.tamanho } })
                .then(cacamba => {
                    if (cacamba == null) {
                        return Cacamba.create(req.body);
                    }
                    return Promise.resolve(cacamba);
                })
                .then(cacamba => {
                    req.body.CacambaId = cacamba.id
                    CacambaPreco.create(req.body)
                        .then(cacambaPreco => {
                            res.json({
                                id: cacambaPreco.id,
                                descricao: cacambaPreco.descricao,
                                tamanho: cacamba.tamanho,
                                quantidade: cacamba.quantidade,
                                valor: cacambaPreco.valor
                            });
                        })
                        .catch(error => {
                            res.status(412).json({
                                mensagem: error.message,
                                sucesso: false
                            });
                        });
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        editarCacamba: (req, res) => {
            if (req.body.tamanho) {
                res.status(412).json({
                    mensagem: "Não é possível alterar o tamanho da caçamba, por favor adicione uma nova.",
                    sucesso: false
                });
            } else if (req.body.quantidade) {
                CacambaPreco.findById(req.params.id)
                    .then(cacamba => Cacamba.update({ quantidade: req.body.quantidade }, {
                        where: { id: cacamba.cacambaId }
                    }))
                    .then(() => res.json({
                        mensagem: "Caçamba alterada com sucesso.",
                        sucesso: true
                    }))
                    .catch(error => res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    }));
            } else {
                CacambaPreco.update(req.body, {
                        where: { id: req.params.id }
                    })
                    .then(response => res.json({
                        mensagem: "Caçamba alterada com sucesso.",
                        sucesso: true
                    }))
                    .catch(error => res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    }))
            }
        },

        removerCacamba: (req, res) => {
            Pedidos.findAll({ where: { cacambaId: req.params.id } })
                .then(pedidos => {
                    if (pedidos.length > 0) {
                        res.status(412).json({
                            mensagem: "Não é possível remover caçambas que possuem pedidos.",
                            sucesso: false
                        })
                    } else {
                        return CacambaPreco.findById(req.params.id);
                    }
                })
                .then(cacamba => cacamba.destroy({ force: true }))
                .then(() => res.json({
                    mensagem: "Cacamba removida com sucesso",
                    sucesso: true
                }))
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    })
                });
        }
    }
}
