import bodyParser from 'body-parser';
import cors from 'cors';

module.exports = app => {
    app.set('port', process.env.PORT || 3000);
    app.set('json spaces', 4);
    app.use(cors());
    app.use(bodyParser.json());
    app.use(app.auth.initialize());
    app.use((req, res, next) => {
        delete req.body.id;
        next();
    });
};