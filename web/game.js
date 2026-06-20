let module = null;
let selected_square = null;     //? {row, col} or null
let board_string = '';
const board_div = document.getElementById("board");
let pending_promotion = null;   //? {fromRow, fromCol, toRow, toCol}
let chosen_mode = null;
let chosen_difficulty = null;
let chosen_color = null;
let last_move = null;           //? {fromRow, fromCol, toRow, toCol}
let is_viewing_history = false;

ChessModule().then(m => {
    module = m;
    renderHome();

    document.getElementById("play").addEventListener("click", () => renderMode());
    document.getElementById("back-mode").addEventListener("click", () => renderHome());
    document.getElementById("PvP").addEventListener("click", () => { chosen_mode = 0; startNewGame(); });
    document.getElementById("PvE").addEventListener("click", () => { chosen_mode = 1; renderDifficulty(); });
    document.getElementById("back-difficulty").addEventListener("click", () => renderMode());
    document.getElementById("easy").addEventListener("click", () => { chosen_difficulty = 0; renderColor(); });
    document.getElementById("medium").addEventListener("click", () => { chosen_difficulty = 1; renderColor(); });
    document.getElementById("hard").addEventListener("click", () => { chosen_difficulty = 2; renderColor(); });
    document.getElementById("back-color").addEventListener("click", () => renderDifficulty());
    document.getElementById("white").addEventListener("click", () => { chosen_color = 0; startNewGame(); });
    document.getElementById("black").addEventListener("click", () => { chosen_color = 1; startNewGame(); });
    document.getElementById("random").addEventListener("click", () => { chosen_color = Math.random() < 0.5 ? 0 : 1; startNewGame(); });
    document.getElementById("quit-game").addEventListener("click", () => renderHome());
    document.getElementById("close-overlay").addEventListener("click", () => {
        document.getElementById("game-over").classList.add("hidden");
        document.getElementById("overlay-blocker").classList.add("hidden");
    });
    document.getElementById("play-again").addEventListener("click", () => {
        document.getElementById("game-over").classList.add("hidden");
        document.getElementById("overlay-blocker").classList.add("hidden");
        startNewGame();
    });
    document.getElementById("quit-over").addEventListener("click", () => {
        document.getElementById("game-over").classList.add("hidden");
        document.getElementById("overlay-blocker").classList.add("hidden");
        renderHome();
    });
    document.getElementById("move-start").addEventListener("click", () => {
        module.goToStart();
        is_viewing_history = (module.historyIndex() !== module.historySize());
        renderBoard(false);
    });
    document.getElementById("move-back").addEventListener("click", () => {
        module.goBack();
        is_viewing_history = (module.historyIndex() !== module.historySize());
        renderBoard(false);
    });
    document.getElementById("move-forward").addEventListener("click", () => {
        module.goForward();
        is_viewing_history = (module.historyIndex() !== module.historySize());
        renderBoard(false);
    });
    document.getElementById("move-end").addEventListener("click", () => {
        module.goToEnd();
        is_viewing_history = false;
        renderBoard(false);
    });

    ['N', 'B', 'R', 'Q'].forEach(piece => {
        document.getElementById(piece).addEventListener("click", () => {
            document.getElementById("promotion-picker").classList.add("hidden");
            document.getElementById("overlay-blocker").classList.add("hidden");
            module.applyMove(pending_promotion.fromRow, pending_promotion.fromCol, pending_promotion.toRow, pending_promotion.toCol, piece);
            renderBoard();
            pending_promotion = null;
            checkGameOver();
        });
    });
});

function renderHome()
{
    showScreen("home");
    chosen_mode = 0;
    chosen_difficulty = 0;
    chosen_color = 0;
}

function renderMode()
{
    showScreen("mode");
}

function renderDifficulty()
{
    showScreen("difficulty");
}

function renderColor()
{
    showScreen("color");
}

function startNewGame()
{
    last_move = null;
    module.startGame(chosen_mode, chosen_difficulty, (chosen_color === 0) ? 1 : 0);
    showScreen("game");
    renderBoard(true);
}

function renderBoard(new_game)
{
    // Saves the board string and div
    board_string = module.getBoard();
    board_div.innerHTML = '';       //* To empty the #board div

    // Creates the cells
    const from_row = chosen_color === 0 ? 7 : 0;
    const to_row = chosen_color === 0 ? -1 : 8;
    const from_col = chosen_color === 0 ? 0 : 7;
    const to_col = chosen_color === 0 ? 8 : -1;
    const step_row = chosen_color === 0 ? -1 : +1;
    const step_col = chosen_color === 0 ? +1 : -1;
    for (let row = from_row; row !== to_row; row += step_row)
        for (let col = from_col; col != to_col; col += step_col)
        {
            const cell = document.createElement("div");
            const piece = board_string[row * 8 + col];
            cell.textContent = (piece === '.') ? '' : piece;
            cell.addEventListener("click", () => handleClick(row, col));    //? If the cell is clicked, it does an action (calling a function in this case)
            board_div.appendChild(cell);

            if ((row + col) % 2)
                cell.classList.add("light");    //? Adds a CSS class 'dark' to the cell
            else
                cell.classList.add("dark");     //? Adds a CSS class 'light' to the cell
        }

    if (last_move && !new_game && !is_viewing_history)
    {
        if (chosen_color)
        {
            board_div.children[last_move.fromRow * 8 + (7 - last_move.fromCol)].classList.add("moved");
            board_div.children[last_move.toRow * 8 + (7 - last_move.toCol)].classList.add("moved");
        }
        else
        {
            board_div.children[(7 - last_move.fromRow) * 8 + last_move.fromCol].classList.add("moved");
            board_div.children[(7 - last_move.toRow) * 8 + last_move.toCol].classList.add("moved");
        }
    }
};

function handleClick(row, col)
{
    if (is_viewing_history) return;

    clearHighlights();
    last_move = null;

    const piece = board_string[row * 8 + col];
    const turn = (chosen_mode === 0) ? module.getCurrentTurn() : chosen_color;
    let cell = null;
    if (chosen_color)
        cell = board_div.children[row * 8 + (7 - col)];
    else
        cell = board_div.children[(7 - row) * 8 + col];

    if (piece !== '.' && ((turn === 0 && piece === piece.toUpperCase()) || (turn === 1 && piece === piece.toLowerCase())))      //* If clicks a friendly piece
    {
        if (selected_square !== null && selected_square.row == row && selected_square.col == col)
        {
            clearHighlights();
            selected_square = null;
        }
        else
        {
            selected_square = {row, col};
            cell.classList.add("selected");

            const names = {'P':"Pawn", 'R':"Rook", 'N':"Knight", 'B':"Bishop", 'Q':"Queen", 'K':"King"};
            const label = document.createElement("div");
            label.textContent = names[piece.toUpperCase()];
            label.classList.add("piece-label");
            cell.appendChild(label);
            setTimeout(() => label.remove(), 750);          //? Disappears after .75 sec

            const legal_moves = module.getLegalMoves(row, col);
            const temp = legal_moves.split('|');
            temp.forEach(coords => {
                if (!coords)    return;     //! Because the last element is an empty string

                let target_cell;
                if (chosen_color)
                    target_cell = board_div.children[Number(coords[0]) * 8 + (7 - Number(coords[2]))];
                else
                    target_cell = board_div.children[(7 - Number(coords[0])) * 8 + Number(coords[2])];

                if (board_string[Number(coords[0]) * 8 + Number(coords[2])] === '.')
                    target_cell.classList.add("legal-empty");
                else
                    target_cell.classList.add("legal-capture");
            });
        }
    }
    else
    {
        if (piece !== '.' && ((turn === 0 && piece === piece.toLowerCase()) || (turn === 1 && piece === piece.toUpperCase())))      //* If clicks an enemy piece that can't eat
        {
            const names = {'P':"Pawn", 'R':"Rook", 'N':"Knight", 'B':"Bishop", 'Q':"Queen", 'K':"King"};
            const label = document.createElement("div");
            label.textContent = names[piece.toUpperCase()];
            label.classList.add("piece-label");
            cell.appendChild(label);
            setTimeout(() => label.remove(), 750);          //? Disappears after 1.5 sec
        }

        if (selected_square === null)   return;
        let exist = false, promotion = false;
        const legal_moves = module.getLegalMoves(selected_square.row, selected_square.col);
        const temp = legal_moves.split('|');
        temp.forEach(coords => {
            if (!coords)    return;     //! Because the last element is an empty string
            if (Number(coords[0]) === row && Number(coords[2]) === col)   //! Index 1 and 3 are ','
            {
                exist = true;
                console.log("Mossa mia: " + coords);
            }
            if (coords[4] === '4')
                promotion = true;
        });
        if (!exist)
        {
            cell.classList.remove("selected");
            selected_square = null;
        }
        else
        {
            if (promotion)
            {
                pending_promotion = {fromRow: selected_square.row, fromCol: selected_square.col, toRow: row, toCol: col};
                selected_square = null;
                document.getElementById("promotion-picker").classList.remove("hidden");
                document.getElementById("overlay-blocker").classList.remove("hidden");
                return;
            }
            module.applyMove(selected_square.row, selected_square.col, row, col, 'P');
            renderBoard(false);
            let starting_cell, arrival_cell;
            if (chosen_color)
            {
                starting_cell = board_div.children[selected_square.row * 8 + (7 - selected_square.col)];
                arrival_cell = board_div.children[row * 8 + (7 - col)];
            }
            else
            {
                starting_cell = board_div.children[(7 - selected_square.row) * 8 + selected_square.col];
                arrival_cell = board_div.children[(7 - row) * 8 + col];
            }
            starting_cell.classList.add("moved");
            arrival_cell.classList.add("moved");
            last_move = {fromRow: selected_square.row, fromCol: selected_square.col, toRow: row, toCol: col};
            selected_square = null;

            if (chosen_mode && module.getGameStatus() === 1)        //? PvE
            {
                setTimeout(() => {
                    const ai_move_str = module.getAiMove();
                    console.log("Mossa ai: " + ai_move_str);
                    if (!ai_move_str) return;
                    const parts = ai_move_str.split(',');
                    module.applyMove(Number(parts[0]), Number(parts[1]), Number(parts[2]), Number(parts[3]), 'Q');
                    last_move = {fromRow: Number(parts[0]), fromCol: Number(parts[1]), toRow: Number(parts[2]), toCol: Number(parts[3])};
                    renderBoard(false);
                    checkGameOver();
                }, 500);
            }

            if (chosen_mode === 0)      //* To rotate the board every turn
            {
                chosen_color = turn === 0 ? 1 : 0;
                const blocker = document.getElementById("overlay-blocker");
                blocker.style.backgroundColor = "transparent";
                blocker.classList.remove("hidden");
                setTimeout(() => { blocker.style.backgroundColor = ""; blocker.classList.add("hidden"); renderBoard(); }, 1000);
            }
        }
    }

    checkGameOver();
};

function clearHighlights()
{
    for (let i = 0; i < 8; i++)
        for (let j = 0; j < 8; j++)
        {
            board_div.children[i * 8 + j].classList.remove("selected");
            board_div.children[i * 8 + j].classList.remove("legal-empty");
            board_div.children[i * 8 + j].classList.remove("legal-capture");
        }
}

function checkGameOver()
{
    const status = module.getGameStatus();
    if (status === 1)   return 0;       //* In Progress
    const message = {
        2: "The winner is white!",
        3: "The winner is black!",
        4: "Draw!",
        5: "Stalemate!",
    };
    document.getElementById("result-message").textContent = message[status];
    document.getElementById("game-over").classList.remove("hidden");
    document.getElementById("overlay-blocker").classList.remove("hidden");
    return 1;
}

function showScreen(id)
{
    ["home", "mode", "difficulty", "color", "game"].forEach (s => {
        document.getElementById("screen-" + s).classList.add("hidden");
    });
    document.getElementById("screen-" + id).classList.remove("hidden");
}