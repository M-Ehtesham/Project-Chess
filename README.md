# Project-Chess
PF project/ semester 1 
At the top of the project, all required libraries are included.
Some **global variables** are also defined because they are needed in multiple functions as well as the main program.

A `char` array is used to store board characters which helps ensure smooth console printing and consistent piece representation.

---

## ♟️ **Console Printing Function**

The first major function prints the board on the console.

* Two nested loops are used to draw the 8×8 board.
* `(8 - i)` is used to print correct row numbering from **bottom to top**, matching actual chess orientation.

---

# ♜ Piece Movement Logic (Phase 3)

This section contains **all movement rules for every chess piece**, implemented manually using conditional checks.

---

## **1. Pawn Movement**

* If it is **white’s turn**, pawns move **upward** (`row - 1`).
* If it is **black’s turn**, pawns move **downward** (`row + 1`).

### **Movement Rules**

* Pawns move straight:

  * 1 step normally
  * 2 steps only from the starting row
* X coordinate must remain the same (straight movement).
* The destination must be empty for normal movement.

### **Capture Logic**

* Pawns capture **diagonally**, meaning:

  * move forward by ±1 in X
  * destination must contain an **enemy** piece

    * lowercase = white’s enemy
    * uppercase = black’s enemy
* If conditions match → valid capture
* Else → invalid move

---

## **2. Bishop Movement**

* Bishop moves **diagonally**.
* The absolute difference of X and Y movement **must be equal**.

### Steps:

1. Calculate the difference in rows and columns.
2. If differences don't match → return false.
3. Determine movement direction:

   * Positive diff = downward
   * Negative diff = upward
4. Use a **while loop** to check every intermediate square:

   * If any piece is blocking → invalid.
5. Check destination:

   * If friendly piece → invalid
   * If enemy → valid capture

---

## **3. Rook Movement**

* Rook moves **horizontally or vertically**.
* Either:

  * `sx == dx` (same column)
  * `sy == dy` (same row)

### Logic:

* Determine row/column direction (+1 or -1)
* Check all squares between source and destination for blocks
* Destination cannot contain friendly piece

---

## **4. Queen Movement**

* Queen = Rook + Bishop combined
* So:

  * If rook-style OR bishop-style movement is valid → queen move valid

---

## **5. King Movement**

* King moves **only 1 square** in any direction.
* Only check:

  * Movement ≤ 1 block
  * Destination must be empty or contain enemy piece

---

## **6. Knight Movement**

* Knight moves in **L-shape**:

  * (2,1) or (1,2)
* Sign does not matter because knight jumps.
* No need to check path blocking.
* Only check:

  * Correct L-shape
  * Destination not containing friendly piece

---

## **7. movePiece()**

This function simply **swaps the source and destination characters**, performing the move on the board.

---

## **8. isValidMove()**

* Used to link characters with their piece movement functions.
* Implemented using a `switch` statement.
* Responsible for selecting correct logic (pawn, rook, bishop, etc.).

---

## **9. Pawn Promotion**

Whenever a pawn reaches the opponent's back rank:

### Process:

1. After every turn, check:

   * Row 0 for white promotion
   * Row 7 for black promotion
2. If a pawn is detected:

   * Ask the user which piece they want
   * Validate input (loop until correct)

---

## **10. FindKing()**

* Scans entire board
* Stores coordinate of:

  * Own king
  * Opponent king

Used in check/checkmate detection.

---

## **11. isSquareAttacked()**

Checks if any square (especially king’s square) is under attack.

### **Pawn Check**

* Check 4 possible diagonal squares.
* If enemy pawn found → return true.

### **Knight Check**

* Check all 8 L-shape positions.
* If enemy knight found → return true.

### **Rook / Queen Check (Straight)**

* Scan horizontally and vertically until:

  * Enemy rook/queen found → true
  * Blocked by another piece → stop

### **Bishop / Queen Check (Diagonal)**

* Four diagonal directions
* Loop in each direction until:

  * Bishop/queen found → true
  * Blocked → stop

### **Opponent King Check**

* Use a 3×3 nested loop
* Check all 8 neighboring squares
* If opponent king found → square is attacked

---

## **12. Checkmate Detection**

### Steps:

1. Check if the king can escape:

   * Try all 8 possible king moves
   * Temporarily move king
   * If still in check → invalid
   * If safe → NOT checkmate

2. Check if any friendly piece can:

   * Capture the attacker
   * Block the check
   * Move legally without causing self-check

Process:

* Loop through all friendly pieces
* For each piece, loop all board squares
* Validate move
* Simulate move temporarily
* Check if king still in check
* If a move removes check → no checkmate

If **no solution found** → Checkmate.

---

# 🏆 Score System

* Score increments when a piece is captured.
* Works both in console and SFML UI.

---

# 🎨 SFML Rendering Explanation

### **Textures**

Used to map characters (`'p'`, `'Q'`, `'k'`, etc.) to **piece images** using a switch-case system.

### **Sprites**

Sprites display the images on the SFML window.

### **Fonts & Text**

Fonts are loaded for:

* Player turn display
* Score display
* Other UI elements

### **Turn Display**

String is updated every turn to show whose turn it is.


# ✔ Summary

This project includes:

* Full piece movement logic
* Pawn promotion
* Check & checkmate system
* Board scanning
* SFML graphics support
* Score and UI updates
* All manual logic 
Just tell me!

