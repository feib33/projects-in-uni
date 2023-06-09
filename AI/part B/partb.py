WHITE, BLACK, CORNER, BLANK, REMOVED = ['O','@','X','-',' ']
ENEMIES = {WHITE: {BLACK, CORNER}, BLACK: {WHITE, CORNER}}
FRIENDS = {WHITE: {WHITE, CORNER}, BLACK: {BLACK, CORNER}}

DIRECTIONS = UP, DOWN, LEFT, RIGHT = (0, -1), (0, 1), (-1, 0), (1, 0)
def step(position, direction):
    """
    Take an (x, y) tuple `position` and a `direction` (UP, DOWN, LEFT or RIGHT)
    and combine to produce a new tuple representing a position one 'step' in
    that direction from the original position.
    """
    px, py = position
    dx, dy = direction
    return (px+dx, py+dy)




class Player():

    def __init__(self,colour):

        self.gameboard=Board();
        self.side='white' if colour == 'white' else 'black'


        






    def action(self,turns):

        if(turns <= 24):
            if(self.side=='black'):
                for i in range(self.gameboard.size):
                    for j in range(2,self.gameboard.size):
                        if(self.gameboard.grid[i,j]=='-'):
                            self.gameboard.grid[i,j]=BLACK
                            return (i,j)
            else:
                for i in range(self.gameboard.size):
                    for j in range(self.gameboard.size-2):
                        if(self.gameboard.grid[i,j]=='-'):
                            self.gameboard.grid[i,j]=WHITE
                            return (i,j)
    def update(self,action):
        x,y=action
        if(self.side == 'white'):
            self.gameboard.grid[x,y]=BLACK
        else :
            self.gameboard.grid[x,y]=WHITE

'''board class for internal board configuration'''
class Board():
    
    def __init__(self):
        """
        Create a new board based on a nested list of characters representing
        an initial board configuration (`data`).
        """
        self.size = 8
        self.grid = {}
        self.white_pieces = []
        self.black_pieces = []
        
        for i in range(self.size):
            for j in range(self.size):
                self.grid[i,j]=BLANK

        for corners in [(0,0),(0,7),(7,0),(7,7)]:
            x,y=corners
            self.grid[x,y]=CORNER
        
              
        """self.black_pieces.append(Piece(BLACK, (x, y), self))"""
    def __str__(self):
        """Compute a string representation of the board's current state."""
        ran = range(self.size)
        return '\n'.join(' '.join(self.grid[x,y] for x in ran) for y in ran)
    
    def find_piece(self, square):
        """
        An O(n) operation (n = number of pieces) to find the piece object
        for the piece occupying a given position on the board. This method
        could be improved by separately keeping track of which piece is at
        each position.
        """
        for piece in self.black_pieces + self.white_pieces:
            if piece.alive and piece.pos == square:
                return piece


class Piece:
    """
    A class to represent a Watch Your Back! piece somewhere on a game board.
    
    This piece tracks its type (BLACK or WHITE, in `player`) and its current 
    position (an (x, y) tuple, in `pos`). It also keeps track of whether or not
    it is currently on the board (Boolean value, in `alive`).

    Contains methods for analysing or changing the piece's current position.
    """
    def __init__(self, player, pos, board):
        """
        Create a new piece for a particular player (BLACK or WHITE) currently
        at a particular position `pos` on board `board`. This piece starts out
        in the `alive = True` state and changes to `alive = False` when it is
        eliminated.
        """
        self.player = player
        self.pos = pos
        self.board = board
        self.alive = True
    def __str__(self):
        return f"{self.player} at {self.pos}"
    def __repr__(self):
        return f"Piece({self.player}, {self.pos})"
    def __eq__(self, other):
        return (self.player, self.pos) == (other.player, other.pos)
    
    def moves(self):
        """
        Compute and return a list of the available moves for this piece based
        on the current board state.

        Do not call with method on pieces with `alive = False`.
        """

        possible_moves = []    
        for direction in DIRECTIONS:
            # a normal move to an adjacent square?
            adjacent_square = step(self.pos, direction)
            if adjacent_square in self.board.grid:
                if self.board.grid[adjacent_square] == BLANK:
                    possible_moves.append(adjacent_square)
                    continue # a jump move is not possible in this direction

            # if not, how about a jump move to the opposite square?
            opposite_square = step(adjacent_square, direction)
            if opposite_square in self.board.grid:
                if self.board.grid[opposite_square] == BLANK:
                    possible_moves.append(opposite_square)
        return possible_moves

    def makemove(self, newpos):
        """
        Carry out a move from this piece's current position to the position
        `newpos` (a position from the list returned from the `moves()` method)
        Update the board including eliminating any nearby pieces surrounded as
        a result of this move.

        Return a list of pieces eliminated by this move (to be passed back to
        the `undomove()` method if you want to reverse this move).

        Do not call with method on pieces with `alive = False`.
        """
        # make the move
        oldpos = self.pos
        self.pos = newpos
        self.board.grid[oldpos] = BLANK
        self.board.grid[newpos] = self.player
        
        # eliminate any newly surrounded pieces
        eliminated_pieces = []

        # check adjacent squares: did this move elimminate anyone?
        for direction in DIRECTIONS:
            adjacent_square = step(self.pos, direction)
            opposite_square = step(adjacent_square, direction)
            if opposite_square in self.board.grid:
                if self.board.grid[adjacent_square] in ENEMIES[self.player] \
                and self.board.grid[opposite_square] in FRIENDS[self.player]:
                    eliminated_piece = self.board.find_piece(adjacent_square)
                    eliminated_piece.eliminate()
                    eliminated_pieces.append(eliminated_piece)

        # check horizontally and vertically: does the piece itself get 
        # eliminated?
        for forward, backward in [(UP, DOWN), (LEFT, RIGHT)]:
            front_square = step(self.pos, forward)
            back_square  = step(self.pos, backward)
            if front_square in self.board.grid \
            and back_square in self.board.grid:
                if self.board.grid[front_square] in ENEMIES[self.player] \
                and self.board.grid[back_square] in ENEMIES[self.player]:
                    self.eliminate()
                    eliminated_pieces.append(self)
                    break
        
        return eliminated_pieces

    def undomove(self, oldpos, eliminated_pieces):
        """
        Roll back a move for this piece to its previous position `oldpos`, 
        restoring the pieces it had eliminated `eliminated_pieces` (a list as
        returned from the `makemove()` method).

        A move should only be 'undone' if no other moves have been made since
        (unless they have already been 'undone' also).

        Do not call with method on pieces with `alive = False` unless you are
        undoing the move that eliminated this piece.
        """
        # put back the pieces that were eliminated
        for piece in eliminated_pieces:
            piece.resurrect()

        # undo the move itself
        newpos = self.pos
        self.pos = oldpos
        self.board.grid[newpos] = BLANK
        self.board.grid[oldpos] = self.player

    def eliminate(self):
        """
        Set a piece's state to `alive = False` and remove it from the board
        For internal use only.
        """
        self.alive = False
        self.board.grid[self.pos] = BLANK

    def resurrect(self):
        """
        Set a piece's state to `alive = True` and restore it to the board
        For internal use only.
        """
        self.alive = True
        self.board.grid[self.pos] = self.player




