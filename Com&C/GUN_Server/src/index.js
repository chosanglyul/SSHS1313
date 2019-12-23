const Koa = require('koa');
const app = new Koa();
const Router = require('koa-router');
const router = new Router();
const client = require('cheerio-httpcli');
const mysql = require('mysql');
const db_config  = require('./config/db-config.json');
const connection = mysql.createConnection({
  host     : db_config.host,
  user     : db_config.user,
  password : db_config.password,
  database : db_config.database
});

router.get('/gun', (ctx, next) => {
    const {id, hp, boost, bullet, armor} = ctx.request.query;
    var query = "UPDATE GUNHP SET hp=" + hp + ", boost=" + boost + ", bullet=" + bullet + ", armor=" + armor + " WHERE id=" + id;
    console.log(query);
    connection.query(query);
    ctx.body = "OK values are updated";
})

app.use(router.routes()).use(router.allowedMethods());

app.listen(4000, () => {
    console.log("The Server Is Listening At Port 4000");
});