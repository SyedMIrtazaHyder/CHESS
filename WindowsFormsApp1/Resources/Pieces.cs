using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Configuration;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1.Resources
{
    internal abstract class Pieces
    {
        protected int r, c;
        protected char name;
        public bool hasMoved;
        protected bool isWhite;
        protected PictureBox img = new PictureBox();
        private static int counter = 0;
        public Pieces(char name, int r, int c, bool isWhite)
        {
            this.name = name;
            this.r = r;
            this.c = c;
            this.isWhite = isWhite;
            hasMoved = false;
            img.BackColor = Color.Transparent;
            img.Size = new Size(45, 45);
            img.TabIndex = counter++;
            img.TabStop = false;
            img.Location = new Point(c*45, r*45);
            img.Name = name.ToString();
            img.SizeMode = PictureBoxSizeMode.StretchImage;
        }
        public abstract int LegalMove(int er, int ec, ref Pieces[] board);

        public PictureBox GetImage()
        {
            return img;
        }

        public char GetName()
        {
            return name;
        }

        public void setRow(int r)
        {
            this.r = r;
            img.Location = new Point(c * 45, r*45);
        }

        public void setCol(int c)
        {
            this.c = c;
            img.Location = new Point(c * 45, r * 45);
        }

        public int getRow()
        {
            return r;
        }

        public int getCol()
        {
            return c;
        }

        public bool isWhitePiece()
        {
            return isWhite;
        }
        public bool isVerticalPathClear(int r, int c, ref Pieces[] board)//will check for pins as well
        {
            bool isKing = false;
            bool isLaser = false;
            for (int j = r + 1; j < 9; j++)
            {
                if (board[8 * (j - 1) + c - 1] != null && board[8 * (j - 1) + c - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (j - 1) + c - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (j - 1) + c - 1] != null && board[8 * (j - 1) + c - 1].isWhitePiece() != isWhite &&
                    (board[8 * (j - 1) + c - 1].GetName().ToString().ToLower() == "q" || board[8 * (j - 1) + c - 1].GetName().ToString().ToLower() == "r"))
                { isLaser = !isLaser; break; }
            }

            for (int j = r - 1; j > 0; j--)
            {
                if (board[8 * (j - 1) + c - 1] != null && board[8 * (j - 1) + c - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (j - 1) + c - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (j - 1) + c - 1] != null && board[8 * (j - 1) + c - 1].isWhitePiece() != isWhite &&
                    (board[8 * (j - 1) + c - 1].GetName().ToString().ToLower() == "q" || board[8 * (j - 1) + c - 1].GetName().ToString().ToLower() == "r"))
                { isLaser = !isLaser; break; }
            }

            if (name.ToString().ToLower() == "k")
                isKing = true;
            return !(isKing && isLaser);
        }
        public bool isHorizontalPathClear(int r, int c, ref Pieces[] board)
        {
            bool isKing = false;
            bool isLaser = false;
            for (int i = c + 1; i < 9; i++)
            {
                if (board[8 * (r - 1) + i - 1] != null && board[8 * (r - 1) + i - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (r - 1) + i - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (r - 1) + i - 1] != null && board[8 * (r - 1) + i - 1].isWhitePiece() != isWhite &&
                    (board[8 * (r - 1) + i - 1].GetName().ToString().ToLower() == "q" || board[8 * (r - 1) + i - 1].GetName().ToString().ToLower() == "r"))
                { isLaser = !isLaser; break; }
            }

            for (int i = c - 1; i > 0; i--)
            {
                if (board[8 * (r - 1) + i - 1] != null && board[8 * (r - 1) + i - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (r - 1) + i - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (r - 1) + i - 1] != null && board[8 * (r - 1) + i - 1].isWhitePiece() != isWhite &&
                    (board[8 * (r - 1) + i - 1].GetName().ToString().ToLower() == "q" || board[8 * (r - 1) + i - 1].GetName().ToString().ToLower() == "r"))
                { isLaser = !isLaser; break; }
            }

            if (name.ToString().ToLower() == "k")
                isKing = true;
            return !(isKing && isLaser);
        }
        public bool isDiagonalPathClear1(int r, int c, ref Pieces[] board)
        {// \
            bool isKing = false;
            bool isSkewer = false;

            for (int j = r + 1, i = c - 1; j < 9 && i > 0; j++, i--)
            {
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() != isWhite &&
                    (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "q" || board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "b"))
                { isSkewer = !isSkewer; break; }
            }

            for (int j = r - 1, i = c + 1; i < 9 && j > 0; i++, j--)
            {
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() != isWhite &&
                    (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "q" || board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "b"))
                { isSkewer = !isSkewer; break; }
            }

            if (name.ToString().ToLower() == "k")
                isKing = true;

            return !(isKing && isSkewer);
        }
        public bool isDiagonalPathClear2(int r, int c, ref Pieces[] board)
        { // /
            bool isKing = false;
            bool isSkewer = false;

            for (int j = r - 1, i = c - 1; j > 0 && i > 0; j--, i--)
            {
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() == isWhite)
                { 
                    if (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "k")
                        isKing = true; 
                    break;
                }
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() != isWhite &&
                    (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "q" || board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "b"))
                { isSkewer = !isSkewer; break; }
            }

            for (int j = r + 1, i = c + 1; i < 9 && j < 9; i++, j++)
            {
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() == isWhite)
                {
                    if (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "k")
                        isKing = true;
                    break;
                }
                if (board[8 * (j - 1) + i - 1] != null && board[8 * (j - 1) + i - 1].isWhitePiece() != isWhite &&
                    (board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "q" || board[8 * (j - 1) + i - 1].GetName().ToString().ToLower() == "b"))
                { isSkewer = !isSkewer; break; }
            }

            if (name.ToString().ToLower() == "k")
                isKing = true;

            return !(isKing && isSkewer);
        }
    }
    internal class Pawn : Pieces
    {
        public Pawn(char name, int r, int c, bool isWhite) : base(name, r, c, isWhite)
        {
            if (isWhite)
                img.Image = Properties.Resources.White_Pawn;
            else
                img.Image = Properties.Resources.Black_Pawn;
        }

        override
        public int LegalMove(int er, int ec, ref Pieces[] board)
        {
            if (er > 8 || er < 1 || ec < 1 || ec > 8)
                return -1;
            int y = er - r;
            int x = ec - c;
            int mul = 1;
            if (isWhite)
                mul = -1;
                //Moving single square
            if (y == mul && x == 0 && board[8 * (er - 1) + ec - 1] == null && isDiagonalPathClear1(r,c,ref board) && isDiagonalPathClear2(r, c, ref board) && isHorizontalPathClear(r, c, ref board))
                return 0;
                //double Push
            if (!hasMoved && y == 2*mul && x == 0 && board[8 * (er - 1) + ec - 1] == null && board[8 * (er - 1 - mul) + ec - 1] == null && isDiagonalPathClear1(r, c, ref board) && isDiagonalPathClear2(r, c, ref board) && isHorizontalPathClear(r, c, ref board))
                return 0;
            //capture
            if (y == mul && Math.Abs(x) == 1 && board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite && isVerticalPathClear(r, c, ref board) && isHorizontalPathClear(r, c, ref board))
            {
                if ((y != x && !isDiagonalPathClear2(r, c, ref board)) || (y == x && !isDiagonalPathClear1(r, c, ref board)))
                    return -1;
                return 1;
            }
            return -1;

        
        }
    }
    internal class King : Pieces
    {
        public King(char name, int r, int c, bool isWhite) : base(name, r, c, isWhite)
        {
            if (isWhite)
                img.Image = Properties.Resources.White_King;
            else
                img.Image = Properties.Resources.Black_King;
        }

        public bool isHorseCheck(int r, int c, ref Pieces[] board)
        {
            if (
                (r + 2 < 9 && c + 1 < 9 && board[8 * (r - 1 + 2) + c - 1 + 1] != null && board[8 * (r - 1 + 2) + c - 1 + 1].isWhitePiece() != isWhite && board[8 * (r - 1 + 2) + c - 1 + 1].GetName().ToString().ToLower() == "n") ||
                (r + 2 < 9 && c - 1 > 0 && board[8 * (r - 1 + 2) + c - 1 - 1] != null && board[8 * (r - 1 + 2) + c - 1 - 1].isWhitePiece() != isWhite && board[8 * (r - 1 + 2) + c - 1 - 1].GetName().ToString().ToLower() == "n") ||
                (r - 2 > 0 && c - 1 > 0 && board[8 * (r - 1 - 2) + c - 1 - 1] != null && board[8 * (r - 1 - 2) + c - 1 - 1].isWhitePiece() != isWhite && board[8 * (r - 1 - 2) + c - 1 - 1].GetName().ToString().ToLower() == "n") ||
                (r - 2 > 0 && c + 1 < 9 && board[8 * (r - 1 - 2) + c - 1 + 1] != null && board[8 * (r - 1 - 2) + c - 1 + 1].isWhitePiece() != isWhite && board[8 * (r - 1 - 2) + c - 1 + 1].GetName().ToString().ToLower() == "n") ||
                (r + 1 < 9 && c + 2 < 9 && board[8 * (r - 1 + 1) + c - 1 + 2] != null && board[8 * (r - 1 + 1) + c - 1 + 2].isWhitePiece() != isWhite && board[8 * (r - 1 + 1) + c - 1 + 2].GetName().ToString().ToLower() == "n") ||
                (r + 1 < 9 && c - 2 > 0 && board[8 * (r - 1 + 1) + c - 1 - 2] != null && board[8 * (r - 1 + 1) + c - 1 - 2].isWhitePiece() != isWhite && board[8 * (r - 1 + 1) + c - 1 - 2].GetName().ToString().ToLower() == "n") ||
                (r - 1 > 0 && c + 2 < 9 && board[8 * (r - 1 - 1) + c - 1 + 2] != null && board[8 * (r - 1 - 1) + c - 1 + 2].isWhitePiece() != isWhite && board[8 * (r - 1 - 1) + c - 1 + 2].GetName().ToString().ToLower() == "n") ||
                (r - 1 > 0 && c - 2 > 0 && board[8 * (r - 1 - 1) + c - 1 - 2] != null && board[8 * (r - 1 - 1) + c - 1 - 2].isWhitePiece() != isWhite && board[8 * (r - 1 - 1) + c - 1 - 2].GetName().ToString().ToLower() == "n"))
                return true;
            return false;

        }

        public bool isChecked(int r, int c, ref Pieces[] board)
        {
            return isHorseCheck(r, c, ref board) || !isDiagonalPathClear1(r, c, ref board) || !isDiagonalPathClear2(r, c, ref board) || !isHorizontalPathClear(r, c, ref board)
                || !isVerticalPathClear(r, c, ref board);
        }

        override
        public int LegalMove(int er, int ec, ref Pieces[] board)
        {
            if (er > 8 || er < 1 || ec < 1 || ec > 8)
                return -1;
            int x = ec - c;
            int y = er - r;

            if (!hasMoved && y == 0 && Math.Abs(x) == 2)//castling
            {
                if (x < 0 && board[8 * (r - 1) + c - 2] == null && board[8 * (r - 1) + c - 3] == null && board[8 * (r - 1) + c - 4] == null && board[8 * (r - 1) + c - 5] != null && !board[8 * (r - 1) + c - 5].hasMoved &&
                    !isChecked(r, 4, ref board) && !isChecked(r, 3, ref board) && !isChecked(r, 2, ref board))
                    return -2;//Queen Side castling
                if ( x > 0 && board[8 * (r - 1) + c] == null && board[8 * (r - 1) + c + 1] == null && board[8 * (r - 1) + c + 2] != null && !board[8 * (r - 1) + c + 2].hasMoved &&
                    !isChecked(r, 7, ref board) && !isChecked(r, 6, ref board))
                    return 2;//King side Castling

            }
            if (Math.Abs(x) <= 1 && Math.Abs(y) <= 1) {
                if (board[8 * (er - 1) + ec - 1] == null && !isChecked(er, ec, ref board))
                    return 0;
                else if(board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite && !isChecked(er, ec, ref board))
                    return 1;
            }
            return -1;
        }
    }
    internal class Bishop : Pieces
    {
        public Bishop(char name, int r, int c, bool isWhite) : base(name, r, c, isWhite)
        {
            if (isWhite)
                img.Image = Properties.Resources.White_Bishop;
            else
                img.Image = Properties.Resources.Black_Bishop;
        }

        override
        public int LegalMove(int er, int ec, ref Pieces[] board)
        {
            if (er > 8 || er < 1 || ec < 1 || ec > 8)
                return -1;
            int y = er - r;
            int x = ec - c;

            if (Math.Abs(y) == Math.Abs(x) && isVerticalPathClear(r, c, ref board) && isHorizontalPathClear(r, c, ref board))
            {
                int NS = 1, EW = 1;
                if (x < 0)
                    EW = -1;
                if (y < 0)
                    NS = -1;
                if ((NS != EW && !isDiagonalPathClear2(r, c, ref board)) || (NS == EW && !isDiagonalPathClear1(r, c, ref board)))
                    return -1;
                for (int i = r + NS, j = c + EW; i != er && j != ec; i += NS, j += EW)
                    if (board[8 * (i - 1) + j - 1] != null)
                        return -1;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return 1;
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                return -1;
            }

            return -1;
        }
    }
    internal class Rook : Pieces
    {
        public Rook(char name, int r, int c, bool isWhite) : base(name, r, c, isWhite)
        {
            if (isWhite)
                img.Image = Properties.Resources.White_Rook;
            else
                img.Image = Properties.Resources.Black_Rook;
        }

        override
        public int LegalMove(int er, int ec, ref Pieces[] board)
        {
            if (er > 8 || er < 1 || ec < 1 || ec > 8)
                return -1;
            int y = er - r;
            int x = ec - c;

            //horizontal movement
            if (y == 0 && isDiagonalPathClear1(r, c, ref board) && isDiagonalPathClear2(r, c, ref board) && isVerticalPathClear(r, c, ref board))
            {
                int s = 1;
                if (x < 0)
                    s = -1;
                for (int temp = c + s; temp != ec; temp += s)//obstacle b/w sliding
                    if (board[8 * (er - 1) + temp - 1] != null)
                        return -1;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return 1;
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                return -1;
            }

            else if (x == 0 && isDiagonalPathClear1(r, c, ref board) && isDiagonalPathClear2(r, c, ref board) && isHorizontalPathClear(r, c, ref board))
            {
                int s = 1;
                if (y < 0)
                    s = -1;
                for (int temp = r + s; temp != er; temp += s)//obstacle b/w sliding
                    if (board[8 * (temp - 1) + ec - 1] != null)
                        return -1;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return 1;
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                return -1;
            }

            return -1;
        }
    }
    internal class Queen : Pieces
    {
        public Queen(char name, int r, int c, bool isWhite) : base(name, r, c, isWhite)
        {
            if (isWhite)
                img.Image = Properties.Resources.White_Queen;
            else
                img.Image = Properties.Resources.Black_Queen;
        }

        override
        public int LegalMove(int er, int ec, ref Pieces[] board)
        {
            if (er > 8 || er < 1 || ec < 1 || ec > 8)
                return -1;
            int y = er - r;
            int x = ec - c;

            //diagonal
            if (Math.Abs(y) == Math.Abs(x) && isVerticalPathClear(r , c, ref board) && isHorizontalPathClear (r, c, ref board))
            {
                int NS = 1, EW = 1;
                if (x < 0)
                    EW = -1;
                if (y < 0)
                    NS = -1;
                if ((NS != EW && !isDiagonalPathClear2(r, c, ref board)) || (NS == EW && !isDiagonalPathClear1(r, c, ref board)))
                    return -1;
                for (int i = r + NS, j = c + EW; i != er && j != ec; i += NS, j += EW)
                    if (board[8 * (i - 1) + j - 1] != null)
                        return -1;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return 1;
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                return -1;
            }
            //horizontal movement
            if (y == 0 && isDiagonalPathClear1(r, c, ref board) && isDiagonalPathClear2(r, c, ref board) && isVerticalPathClear(r, c, ref board))
            {
                int s = 1;
                if (x < 0)
                    s = -1;
                for (int temp = c + s; temp != ec; temp += s)//obstacle b/w sliding
                    if (board[8 * (er - 1) + temp - 1] != null)
                        return -1;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return 1;
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                return -1;
            }
            //vertical movement
            else if (x == 0 && isDiagonalPathClear1(r, c, ref board) && isDiagonalPathClear2(r, c, ref board) && isHorizontalPathClear(r, c, ref board))
            {
                int s = 1;
                if (y < 0)
                    s = -1;
                for (int temp = r + s; temp != er; temp += s)//obstacle b/w sliding
                    if (board[8 * (temp - 1) + ec - 1] != null)
                        return -1;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return 1;
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                return -1;
            }

            return -1;
        }
    }
    internal class Knight : Pieces
    {
        public Knight(char name, int r, int c, bool isWhite) : base(name, r, c, isWhite)
        {
            if (isWhite)
                img.Image = Properties.Resources.White_Knight;
            else
                img.Image = Properties.Resources.Black_Knight;
        }

        override
        public int LegalMove(int er, int ec, ref Pieces[] board)
        {
            if (er > 8 || er < 1 || ec < 1 || ec > 8 ||
                !isDiagonalPathClear1(r,c, ref board) || !isDiagonalPathClear2(r, c, ref board) || !isHorizontalPathClear(r, c, ref board) || !isVerticalPathClear(r, c, ref board))
                return -1;
            int y = er - r;
            int x = ec - c;

            if ((Math.Abs(x) == 2 && Math.Abs(y) == 1) || (Math.Abs(x) == 1 && Math.Abs(y) == 2))
                {
                if (board[8 * (er - 1) + ec - 1] == null)
                    return 0;
                 else if (board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                        return 1;
                }
            return -1;

        }
    }
}
