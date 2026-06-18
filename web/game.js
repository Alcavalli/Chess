let module = null;
let selected_square = null;     //? {row, col} or null
let board_string = '';
const board_div = document.getElementById("board");
let pending_promotion = null;   //? {fromRow, fromCol, toRow, toCol}

ChessModule().then(m => {
    module = m;
    renderBoard();
    document.getElementById("close-overlay").addEventListener("click", () => {
        document.getElementById("game-over").classList.add("hidden");
    });
    document.getElementById("overlay-blocker").classList.add("hidden");
    document.getElementById("play-again").addEventListener("click", () => location.reload());
    document.getElementById("quit").addEventListener("click", () => location.reload());

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

function renderBoard()
{
    // Saves the board string and div
    board_string = module.getBoard();
    board_div.innerHTML = '';       //* To empty the #board div

    // Creates the cells
    for (let row = 7; row >= 0; row--)      //? Start from 7 so that black is up
        for (let col = 0; col < 8; col++)
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
};

function handleClick(row, col)
{
    clearHighlights();
    const piece = board_string[row * 8 + col];
    const turn = module.getCurrentTurn();
    const cell = board_div.children[(7 - row) * 8 + col];

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
                const target_cell = board_div.children[(7 - Number(coords[0])) * 8 + Number(coords[2])];
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
                exist = true;
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
            renderBoard();
            const starting_cell = board_div.children[(7 - selected_square.row) * 8 + selected_square.col];
            const arrival_cell = board_div.children[(7 - row) * 8 + col];
            starting_cell.classList.add("moved");
            arrival_cell.classList.add("moved");
            setTimeout(() => starting_cell.classList.remove("moved"), 1200);
            setTimeout(() => arrival_cell.classList.remove("moved"), 1200);
            selected_square = null;
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
            board_div.children[i * 8 + j].classList.remove("moved");
        }
}

function checkGameOver()
{
    const status = module.getGameStatus();
    if (status === 1 || status === 0)   return 0;       //* In Progress (or menu at the moment)
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