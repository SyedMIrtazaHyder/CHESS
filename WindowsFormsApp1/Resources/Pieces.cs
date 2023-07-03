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
        public abstract bool LegalMove(int er, int ec, ref Pieces[] board);

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
        /*protected bool isHorizontalMove
        protected bool isVerticalMove
        protected bool isDiagonalMove
        protected bool isHorizontalPathClear
        protected isVerticalPathClear
        protected isDiagonalPathClear*/
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
        public bool LegalMove(int er, int ec, ref Pieces[] board)
        {
            int y = er - r;
            int x = ec - c;
            int mul = 1;
            if (isWhite)
                mul = -1;
                //Moving single square
            if (y == mul && x == 0 && board[8 * (er - 1) + ec - 1] == null)
                return true;
                //double Push
            if (!hasMoved && y == 2*mul && x == 0 && board[8 * (er - 1) + ec - 1] == null && board[8 * (er - 1 - mul) + ec - 1] == null)
                return true;
            //capture
            if (y == mul && Math.Abs(x) == 1 && board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                return true;
            return false;

        
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

        override
        public bool LegalMove(int er, int ec, ref Pieces[] board)
        {
            int x = c - ec;
            int y = r - er;

            if (Math.Abs(x) <= 1 && Math.Abs(y) <= 1 && (board[8 * (er - 1) + ec - 1] == null || board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite))
                return true;
            return false;
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
        public bool LegalMove(int er, int ec, ref Pieces[] board)
        {
            int y = er - r;
            int x = ec - c;

            if (Math.Abs(y) == Math.Abs(x))
            {
                int NS = 1, EW = 1;
                if (x > 0)
                    EW = -1;
                if (y > 0)
                    NS = -1;
                for(int i = er, j = ec; i != r && j != c; i+=NS, j+=EW)
                    if (board[8*(i-1) + j - 1] != null)
                        return false;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() == isWhite)
                    return false;
                return true;

            }

            return false;
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
        public bool LegalMove(int er, int ec, ref Pieces[] board)
        {
            int y = er - r;
            int x = ec - c;

            if (y == 0)//horizontal movement
            {
                int s = 1;
                if (x > 0)
                    s = -1;
                for (int temp = ec; temp != c; temp += s)//obstacle b/w sliding
                    if (board[8 * (er - 1) + temp - 1] != null)
                        return false;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() == isWhite)
                    return false;
                return true;
            }

            else if (x == 0)
            { 
                int s = 1;
                if (y > 0)
                    s = -1;
                for (int temp = er; temp != r; temp += s)//obstacle b/w sliding
                    if (board[8 * (temp - 1) + ec - 1] != null)
                        return false;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() == isWhite)
                    return false;
                return true;
            }

            return false;
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
        public bool LegalMove(int er, int ec, ref Pieces[] board)
        {
            int y = er - r;
            int x = ec - c;

            //diagonal
            if (Math.Abs(y) == Math.Abs(x))
            {
                int NS = 1, EW = 1;
                if (x > 0)
                    EW = -1;
                if (y > 0)
                    NS = -1;
                for (int i = er, j = ec; i != r && j != c; i += NS, j += EW)
                    if (board[8 * (i - 1) + j - 1] != null)
                        return false;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() == isWhite)
                    return false;
                return true;

            }

            else if (y == 0)//horizontal movement
            {
                int s = 1;
                if (x > 0)
                    s = -1;
                for (int temp = ec; temp != c; temp += s)//obstacle b/w sliding
                    if (board[8 * (er - 1) + temp - 1] != null)
                        return false;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() == isWhite)
                    return false;
                return true;
            }

            else if (x == 0)
            {
                int s = 1;
                if (y > 0)
                    s = -1;
                for (int temp = er; temp != r; temp += s)//obstacle b/w sliding
                    if (board[8 * (temp - 1) + ec - 1] != null)
                        return false;
                if (board[8 * (er - 1) + ec - 1] != null && board[8 * (er - 1) + ec - 1].isWhitePiece() == isWhite)
                    return false;
                return true;
            }

            return false;
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
        public bool LegalMove(int er, int ec, ref Pieces[] board)
        {
            int y = er - r;
            int x = ec - c;

            if ((Math.Abs(x) == 2 && Math.Abs(y) == 1) || (Math.Abs(x) == 1 && Math.Abs(y) == 2))
                if(board[8 * (er - 1) + ec - 1] == null || board[8 * (er - 1) + ec - 1].isWhitePiece() != isWhite)
                    return true;
            return false;

        }
    }

}
