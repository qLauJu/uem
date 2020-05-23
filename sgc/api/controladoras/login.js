import jwt from "jwt-simple";

module.exports = app => {
    const config = app.libs.config;
    const Usuario = app.db.models.Usuario;

    return {
        login: (req, res) => {
            if (req.body.documento && req.body.senha) {
                const documento = req.body.documento;
                const senha = req.body.senha;
                // res.json(req.body);
                Usuario.findOne({ where: { documento: documento } })
                    .then(usuario => {
                        // res.json(usuario);
                        if (Usuario.isPassword(usuario.senha, senha)) {
                            const payload = { id: usuario.id };
                            res.json({
                                tipo: usuario.tipo,
                                token: jwt.encode(payload, config.jwtSecret)
                            });
                        } else {
                            res.status(401).send({
                                mensagem: "Usuário não autorizado.",
                                sucesso: false
                            });
                        }
                    })
                    .catch(error => {
                        res.status(401).send({
                                mensagem: "Usuário não encontrado.",
                                sucesso: false
                            })
                        });
            } else {
                res.status(412).send({
                    mensagem: "Documento ou senha não enviados.",
                    sucesso: false
                });
            }
        }
    }
}
