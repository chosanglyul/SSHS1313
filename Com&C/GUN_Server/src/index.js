const Koa = require('koa');
const app = new Koa();
const Router = require('koa-router');
const router = new Router();
const mysql = require('mysql');
const db_config  = require('./config/db-config.json');
const connection = mysql.createConnection({
  host     : db_config.host,
  user     : db_config.user,
  password : db_config.password,
  database : db_config.database
});

router.get('/gun', (ctx, next) => {
  const {id, fset, hp, gauge, maxblit, blit, totblit, armor, silencer} = ctx.request.query;
  var query = "UPDATE GUNHP SET fset=" + fset + ", hp=" + hp + ", gauge=" + gauge + ", maxblit=" + maxblit + ", blit=" + blit + ", totblit=" + totblit + ", armor=" + armor + ", silencer=" + silencer + " WHERE id=" + id;
  console.log(query);
  connection.query(query);
  ctx.body = "gun OK values are updated";
})

router.get('/deal', (ctx, next) => {
  const {id, dealer, deal} = ctx.request.query;
  var query = "UPDATE Deal SET deal=deal+" + deal + " WHERE id=" + dealer;
  console.log(query);
  connection.query(query);
  query = "UPDATE Deal SET damage=damage+" + deal + " WHERE id=" + id;
  console.log(query);
  connection.query(query);
  ctx.body = "deal OK values are updated";
})

router.get('/kill', (ctx, next) => {
  const {id, killer} = ctx.request.query;
  var query = "INSERT INTO Killlog (killer, dead) VALUES (" + killer + ", " + id + ")";
  console.log(query);
  connection.query(query);
  ctx.body = "kill OK values are updated";
})

app.use(router.routes()).use(router.allowedMethods());

app.listen(4000, () => {
    console.log("The Server Is Listening At Port 4000");
});