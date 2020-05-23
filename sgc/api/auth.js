import passport from 'passport';
import { Strategy, ExtractJwt } from 'passport-jwt';

module.exports = app => {
    const Usuario = app.db.models.Usuario;
    const config = app.libs.config;
    const params = {
        secretOrKey: config.jwtSecret,
        jwtFromRequest: ExtractJwt.fromAuthHeaderAsBearerToken()
    };

    const strategy = new Strategy(params, (payload, done) => {
        Usuario.findById(payload.id)
            .then(usuario => {
                if (usuario)
                    return done(null, {
                        id: usuario.id,
                        documento: usuario.documento,
                        tipo: usuario.tipo
                    });
                return done(null, false);
            })
            .catch(error => done(error, null));
    });

    passport.use(strategy);
    
    return {
        initialize: () => {
            return passport.initialize();
        },
        authenticate: () => {
            return passport.authenticate('jwt', config.jwtSession);
        },
        authorize: (...tipos) => {
            return (req, res, next) => {
                const tipoUsuarioAtual = req.user.tipo;
                if (tipos.indexOf(tipoUsuarioAtual) == -1) {
                    res.sendStatus(403);
                } else {
                    next();
                }
            }
        }
    };
};