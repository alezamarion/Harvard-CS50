import os
import sys

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db", connect_args={'check_same_thread': False})

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")



@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    id=session["user_id"]

    #create tables if necessary
    db.execute("CREATE TABLE IF NOT EXISTS current ( id INTEGER NOT NULL, symbol TEXT NOT NULL, shares INTEGER NOT NULL)")
    db.execute("CREATE TABLE IF NOT EXISTS history ( id INTEGER NOT NULL, action TEXT NOT NULL, symbol TEXT NOT NULL, shares INTEGER, price INTEGER, stamp TEXT)")
    owns = db.execute("SELECT * FROM current WHERE id=:id", id=id)
    looks=[]
    totals = []

    #extract cash
    cash = int(db.execute("SELECT cash FROM users WHERE id=:id", id=id)[0]['cash'])
    sumtotal=cash

    #search portfolio owned
    for own in owns:
        look = lookup(own['symbol'])
        total = float(own['shares'])*float(look['price'])
        looks.append(look)
        totals.append(total)
        sumtotal = sumtotal+total
    return render_template("index.html", zips=zip(owns,looks,totals), sum=sumtotal,cash=cash)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    #user reached route via GET (clicking a link or via redirect)
    if request.method=="GET":
        return render_template("buy.html")
    else:

        #search for stock
        symbol=request.form['symbol']
        look = lookup(symbol)
        if not look:
            return apology("Please enter valid symbol")
        else:
            shares=float(request.form['shares'])
            price=look['price']
            name=look['name']
            id=session["user_id"]
            balance=float(db.execute("SELECT cash FROM users WHERE id=:id", id=id)[0]['cash'])
            if balance>(price*shares):

                #buying stocks
                db.execute("INSERT INTO history (id,action,symbol,shares,price,stamp) VALUES (:id, :action, :symbol, :shares, :price, datetime('now','localtime'))",id=id, action="BUY", symbol=symbol, shares=shares, price=price)
                curshares = db.execute("SELECT shares FROM current WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=symbol)
                if not curshares:
                    db.execute("INSERT INTO current (id, symbol, shares) VALUES (:id, :symbol, :shares)", id=id, symbol=symbol, shares=shares)
                else:
                    curshares = int(curshares[0]['shares'])
                    curshares = curshares+shares
                    db.execute("UPDATE current SET shares=:curshares WHERE id=:id AND symbol=:symbol", curshares=curshares, id=id, symbol=symbol)
                balance = str(balance-(shares*price))
                db.execute("UPDATE users SET cash=:balance WHERE id=:id", balance=balance, id=session["user_id"])
                return redirect("/")
            else:
                return apology("Transaction Failed")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    #extract history
    id=session["user_id"]
    histories = db.execute("SELECT * FROM history WHERE id=:id ORDER BY stamp DESC", id=id)
    return render_template("history.html", histories=histories)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    #forget any user_id
    session.clear()

    #user reached route via POST
    if request.method == "POST":

        #username was submitted?
        if not request.form.get("username"):
            return apology("must provide username", 403)

        #password was submitted?
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        #query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        #username exists and password is correct?
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        #remember which user has logged in
        session["user_id"] = rows[0]["id"]

        #redirect user to home page
        return redirect("/")

    #user reached route via GET (clicking a link or via redirect)
    else:
        return render_template("login.html")



@app.route("/logout")
def logout():
    """Log user out"""

    #forget any user_id
    session.clear()

    #redirect user to login
    return redirect("/")



@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    #user reached route via GET (clicking a link or via redirect)
    if request.method=="GET":
        return render_template("quote.html")
    else:

        #quoting
        quote = lookup(request.form.get("symbol"))
        print(quote, file=sys.stderr)
        if quote==None:
            return apology("Quote Failed")
        else:
            return render_template('quoted.html', quote=quote)



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    #user reached route via GET (clicking a link or via redirect)
    if request.method=="GET":
        return render_template("register.html")
    else:

        #data required
        username = request.form['username']
        if not username:
            return apology("Must enter username!")
        password = request.form['password']
        if not password:
            return apology("Must enter password!")
        pwd_match = request.form["pwd_match"]
        if pwd_match != password:
            return apology("passwords don't match")

        #hashing password
        phash = generate_password_hash(password)
        rows = db.execute("INSERT INTO users (username,hash) VALUES (:username, :phash)", username=username, phash=phash)
        return redirect("/")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    id=session["user_id"]

    #my stocks
    options = db.execute("SELECT * FROM current WHERE id=:id", id=id)
    if request.method=="GET":
        return render_template("sell.html", options=options)
    else:

        #selling and updating database
        symbol=request.form['symbol']
        shares=int(request.form['shares'])
        curshares=int(db.execute("SELECT shares FROM current WHERE id=:id AND symbol=:symbol", id=id, symbol=symbol)[0]['shares'])
        if shares<=curshares:
            curshares=curshares-shares
            if curshares==0:
                db.execute("DELETE FROM current WHERE id=:id AND symbol=:symbol", id=id, symbol=symbol)
            else:
                db.execute("UPDATE current SET shares=:curshares WHERE id=:id AND symbol=:symbol",curshares=curshares, id=id, symbol=symbol)
            price=float(lookup(symbol)['price'])
            rate=price*shares
            cash=float(db.execute("SELECT cash FROM users WHERE id=:id", id=id)[0]['cash'])
            cash = cash+rate
            db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash, id=id)
            db.execute("INSERT INTO history (id, action, symbol, shares, price, stamp) VALUES (:id, :action, :symbol, :shares, :price, datetime('now', 'localtime'))", id=id, action="SELL", symbol=symbol, shares=shares, price=price)
            return redirect("/")
        else:
            return apology("Selling more than Owned")
        return apology("else reached")



@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """Add funds to account"""

    #user reached route via GET (clicking a link or via redirect)
    if request.method == "GET":
        return render_template("add.html")
    else:

        #declaring value and updating database
        fundsToAdd = request.form['cashToAdd']
        try:
            fundsToAdd = float(fundsToAdd)
        except ValueError:
            return apology("invalid amount")
        if fundsToAdd < 0:
            return apology("Enter a positive cash value")
        db.execute("UPDATE users SET cash = cash + :fundsToAdd WHERE id = :id", fundsToAdd=round(fundsToAdd, 2), id=session["user_id"])
        return render_template("history.html")



def errorhandler(e):
    """Handle error"""

    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)



#listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)