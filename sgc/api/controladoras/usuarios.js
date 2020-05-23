module.exports = app => {
    const Usuario = app.db.models.Usuario;
    const Config = app.db.models.Config;

    return {
        getUsuarios: (req, res) => {
            Usuario.findAll({ where: { tipo: "CLIENTE" } })
                .then(usuarios => {
                    res.json(usuarios.map(usuario => {
                        return {
                            id: usuario.id,
                            documento: usuario.documento,
                            nome: usuario.nome,
                            email: usuario.email
                        }
                    }));
                })
                .catch(error => {
                    res.status(412).json({
                        message: error.message
                    });
                });
        },

        getUsuario: (req, res) => {
            Usuario.findById(req.params.id, {
                attributes: {
                    exclude: ['senha', 'updated_at']
                }
            })
                .then(usuario => {
                    if (usuario) {
                        res.json(usuario)
                    } else {
                        res.sendStatus(404);
                    }
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message
                    });
                });
        },

        getPerfil: (req, res) => {
            Usuario.findById(req.user.id, {
                attributes: {
                    exclude: ['senha', 'updated_at']
                }
            })
                .then(usuario => res.json(usuario))
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message
                    });
                });
        },

        inserirUsuario: (req, res) => {
            Usuario.create(req.body)
                .then(usuario => {
                    Config.create({
                        userId: usuario.id,
                        qtdePadraoDeDias: 5,
                        termosDeUso: '',
                        termosDeUso: '',
                        habilitarNotificacao: true
                    });
                    usuario.senha = undefined;
                    res.json(usuario);
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message,
                        sucesso: false
                    });
                });
        },

        getUsuarios: (req, res) => {
            Usuario.findAll({ where: { tipo: 'CLIENTE' } })
                .then(usuarios => {
                    res.json(usuarios.map(usuario => {
                        return {
                            id: usuario.id,
                            documento: usuario.documento,
                            nome: usuario.nome,
                            email: usuario.email,
                        }
                    }));
                })
                .catch(error => {
                    res.status(412).json({ message: error });
                });
        },

        getUsuario: (req, res) => {
            Usuario.findByID(req.params.id, {
                attributes: { exclude: ['senha', 'updated-at'] }
            })
                .then(usuario => {
                    if (usuario) { res.json(usuario) }
                    else { res.sendStatus(404); }
                })
                .catch(error => {
                    res.status(415).json({ mensagem: error.message });
                });
        },

        getDocumentos: (req, res) => {
            Usuario.findAll({
                where: { tipo: 'CLIENTE' },
                attributes: ['documento']
            })
                .then(documentos => {
                    res.json(documentos.map(doc => {
                        return doc['documento'];
                    }));
                })
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message
                    });
                });
        },

        removerUsuario: (req, res) => {
            Usuario.destroy({
                where: {
                    id: req.user.id
                }
            })
                .then(result => res.sendStatus(204))
                .catch(error => {
                    res.status(412).json({
                        mensagem: error.message
                    });
                });
        }
    };
};
