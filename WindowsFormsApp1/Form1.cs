﻿using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.Remoting.Channels;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WindowsFormsApp1.Resources;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        private PictureBox prevSelected = null;
        private Pieces[] chessBoard = new Pieces[64];
        private Pieces pieceToMove = null;
        private static int turnCounter = 1;
        List<Pieces> attacked = new List<Pieces>();
        King BK = null;
        King WK = null;

        int lastCapturePosition;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //From Top to Bottom so Black Rank 8
            chessBoard[0] = new Rook('r', 1, 1, false);
            chessBoard[1] = new Knight('n', 1, 2, false);
            chessBoard[2] = new Bishop('b', 1, 3, false);
            chessBoard[3] = new Queen('q', 1, 4, false);
            BK = new King('k', 1, 5, false);
            chessBoard[4] = BK;
            chessBoard[5] = new Bishop('b', 1, 6, false);
            chessBoard[6] = new Knight('n', 1, 7, false);
            chessBoard[7] = new Rook('r', 1, 8, false);
            //Black Rank 7
            for (int i = 8; i < 16; i++)
                chessBoard[i] = new Pawn('p', 2, i % 8 + 1, false);

            //White Rank 2
            for (int i = 48; i < 56; i++)
                chessBoard[i] = new Pawn('P', 7, i % 8 + 1, true);

            //White Rank 1
            chessBoard[56] = new Rook('R', 8, 1, true);
            chessBoard[57] = new Knight('N', 8, 2, true);
            chessBoard[58] = new Bishop('B', 8, 3, true);
            chessBoard[59] = new Queen('Q', 8, 4, true);
            WK = new King('K', 8, 5, true);
            chessBoard[60] = WK;
            chessBoard[61] = new Bishop('B', 8, 6, true);
            chessBoard[62] = new Knight('N', 8, 7, true);
            chessBoard[63] = new Rook('R', 8, 8, true);

            foreach (Pieces p in chessBoard)
                if (p != null)
                {
                    p.GetImage().Click += new EventHandler(ClickOnPiece);
                    p.GetImage().Parent = Board;
                }
            ErrorScreen.Text = "CHESS GAME STARTED";
        }

        private void Swap(int loc1, int loc2)
        {
            (chessBoard[loc2], chessBoard[loc1]) = (chessBoard[loc1], chessBoard[loc2]);
        }
        private void ClickOnPiece(object sender, EventArgs e)
        {
            if (((PictureBox)sender).BackColor == Color.LightCoral)//capture logic
            {
                MovePiece(sender, e);
                if (((PictureBox)sender).Location == prevSelected.Location)
                {
                    ((PictureBox)sender).Visible = false;//piece overwritten when new piece on top of it
                    chessBoard[lastCapturePosition] = null;//removing captured piece from board entirely
                }
                return;
            }
            CleanUpMoves();
            if (prevSelected != null)
                prevSelected.BackColor = Color.Transparent;
            prevSelected = (PictureBox)sender;
            pieceToMove = chessBoard[prevSelected.Location.X / 45 - 1 + 8 * (prevSelected.Location.Y / 45 - 1)];
            if (pieceToMove != null &&
                (Turn.Text[6] == 'W' && pieceToMove.isWhitePiece()) ||
                (Turn.Text[6] == 'B' && !pieceToMove.isWhitePiece())
                )
                ((PictureBox)sender).BackColor = Color.Green;
            else
                ((PictureBox)sender).BackColor = Color.Red;

            potentialMoves(pieceToMove);
            //Console.Write("Now on:" + sender.ToString());
        }

        private void MovePiece(object sender, EventArgs e)
        {
            if (pieceToMove != null && prevSelected.BackColor == Color.Red)
            {
                //Console.WriteLine("Invalid Piece Being Moved");
                ErrorScreen.Text = "Invalid Piece Being Moved";
                return;
            }

            //castling

            else if (pieceToMove != null && prevSelected.BackColor == Color.Green)
            {
                Point newPos = ((PictureBox)sender).Location;
                bool flag = SquareBounds(ref newPos);
                int newBoardPos = newPos.X / 45 - 1 + 8 * (newPos.Y / 45 - 1);
                int oldBoardPos = prevSelected.Location.X / 45 - 1 + 8 * (prevSelected.Location.Y / 45 - 1);
                lastCapturePosition = oldBoardPos;
                Console.WriteLine($"{newPos.X / 45}, {newPos.Y / 45}");
                if (flag)
                {
                    Swap(newBoardPos, oldBoardPos);
                    pieceToMove.setRow(newPos.Y / 45);
                    pieceToMove.setCol(newPos.X / 45);
                    if (Turn.Text[6] == 'W')
                    {
                        Turn.Text = "Turn: Black";
                        Turn.BackColor = Color.White;
                        Turn.ForeColor = Color.Black;
                    }
                    else if (Turn.Text[6] == 'B')
                    {
                        Turn.Text = "Turn: White";
                        Turn.BackColor = Color.Black;
                        Turn.ForeColor = Color.White;
                        turnCounter++;
                    }
                    if (!pieceToMove.hasMoved)
                        pieceToMove.hasMoved = true;
                    prevSelected.BackColor = Color.Transparent;
                    ErrorScreen.Text = "Turn: " + turnCounter;
                    if (BK.isChecked(BK.getRow(), BK.getCol(), ref chessBoard) && Turn.Text[6] == 'B')
                            Turn.Text += " in CHECK";
                    else if (WK.isChecked(WK.getRow(), WK.getCol(), ref chessBoard) && Turn.Text[6] == 'W')
                            Turn.Text += " in CHECK";
                    
                }

                else
                    ErrorScreen.Text = "Invalid Move";
                //Console.WriteLine("Invalid Move");
                //chessBoard[newBoardPos].hasMoved = true;
            }
            else
                ErrorScreen.Text = "No Piece Selected";
            CleanUpMoves();
            //Console.WriteLine("No piece Selected");
        }

        private int potentialMoves(Pieces p)
        {
            int mul = 1;
            if (p.isWhitePiece())
                mul = -1;
            switch (p.GetName())
            {
                case 'P':
                case 'p':
                    DisplayPotentialMoves(p.GetName(), p.getRow() + mul, p.getCol(), p.LegalMove(p.getRow() + mul, p.getCol(), ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 2 * mul, p.getCol(), p.LegalMove(p.getRow() + 2 * mul, p.getCol(), ref chessBoard));
                    //captures
                    DisplayPotentialMoves(p.GetName(), p.getRow() + mul, p.getCol() + 1, p.LegalMove(p.getRow() + mul, p.getCol() + 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() + mul, p.getCol() - 1, p.LegalMove(p.getRow() + mul, p.getCol() - 1, ref chessBoard));
                    break;

                case 'R':
                case 'r':
                    for (int i = p.getCol() + 1; i < 9; i++)//right
                    {
                        if (p.LegalMove(p.getRow(), i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), p.getRow(), i, p.LegalMove(p.getRow(), i, ref chessBoard));
                    }
                    for (int i = p.getCol() - 1; i > 0; i--)//left
                    {
                        if (p.LegalMove(p.getRow(), i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), p.getRow(), i, p.LegalMove(p.getRow(), i, ref chessBoard));
                    }
                    for (int j = p.getRow() + 1; j < 9; j++)//up
                    {
                        if (p.LegalMove(j, p.getCol(), ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, p.getCol(), p.LegalMove(j, p.getCol(), ref chessBoard));
                    }
                    for (int j = p.getRow() - 1; j > 0; j--)//down
                    {
                        if (p.LegalMove(j, p.getCol(), ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, p.getCol(), p.LegalMove(j, p.getCol(), ref chessBoard));
                    }
                    break;

                case 'B':
                case 'b':
                    for (int i = p.getCol() + 1, j = p.getRow() + 1; i < 9 && j < 9; i++, j++)//NE
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));
                    }
                    for (int i = p.getCol() - 1, j = p.getRow() + 1; i > 0 && j < 9; i--, j++)//NW
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));

                    }
                    for (int i = p.getCol() + 1, j = p.getRow() - 1; i < 9 && j > 0; i++, j--)//SE
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));

                    }
                    for (int i = p.getCol() - 1, j = p.getRow() - 1; i > 0 && j > 0; i--, j--)//SW
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));
                    }
                    break;

                case 'Q':
                case 'q':
                    for (int i = p.getCol() + 1; i < 9; i++)//right
                    {
                        if (p.LegalMove(p.getRow(), i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), p.getRow(), i, p.LegalMove(p.getRow(), i, ref chessBoard));
                    }
                    for (int i = p.getCol() - 1; i > 0; i--)//left
                    {
                        if (p.LegalMove(p.getRow(), i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), p.getRow(), i, p.LegalMove(p.getRow(), i, ref chessBoard));
                    }
                    for (int j = p.getRow() + 1; j < 9; j++)//up
                    {
                        if (p.LegalMove(j, p.getCol(), ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, p.getCol(), p.LegalMove(j, p.getCol(), ref chessBoard));
                    }
                    for (int j = p.getRow() - 1; j > 0; j--)//down
                    {
                        if (p.LegalMove(j, p.getCol(), ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, p.getCol(), p.LegalMove(j, p.getCol(), ref chessBoard));
                    }
                    for (int i = p.getCol() + 1, j = p.getRow() + 1; i < 9 && j < 9; i++, j++)//NE
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));
                    }
                    for (int i = p.getCol() - 1, j = p.getRow() + 1; i > 0 && j < 9; i--, j++)//NW
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));

                    }
                    for (int i = p.getCol() + 1, j = p.getRow() - 1; i < 9 && j > 0; i++, j--)//SE
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));

                    }
                    for (int i = p.getCol() - 1, j = p.getRow() - 1; i > 0 && j > 0; i--, j--)//SW
                    {
                        if (p.LegalMove(j, i, ref chessBoard) == -1)
                            break;
                        DisplayPotentialMoves(p.GetName(), j, i, p.LegalMove(j, i, ref chessBoard));
                    }
                    break;

                case 'N':
                case 'n':
                    //--|
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 1, p.getCol() + 2, p.LegalMove(p.getRow() + 1, p.getCol() + 2, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 1, p.getCol() + 2, p.LegalMove(p.getRow() - 1, p.getCol() + 2, ref chessBoard));

                    //|--
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 1, p.getCol() - 2, p.LegalMove(p.getRow() + 1, p.getCol() - 2, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 1, p.getCol() - 2, p.LegalMove(p.getRow() - 1, p.getCol() - 2, ref chessBoard));

                    // |
                    //-|
                    // |
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 2, p.getCol() + 1, p.LegalMove(p.getRow() + 2, p.getCol() + 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 2, p.getCol() + 1, p.LegalMove(p.getRow() - 2, p.getCol() + 1, ref chessBoard));

                    //|
                    //|-
                    //|
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 2, p.getCol() - 1, p.LegalMove(p.getRow() + 2, p.getCol() - 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 2, p.getCol() - 1, p.LegalMove(p.getRow() - 2, p.getCol() - 1, ref chessBoard));
                    break;

                default:
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 1, p.getCol(), p.LegalMove(p.getRow() + 1, p.getCol(), ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 1, p.getCol(), p.LegalMove(p.getRow() - 1, p.getCol(), ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow(), p.getCol() + 1, p.LegalMove(p.getRow(), p.getCol() + 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow(), p.getCol() - 1, p.LegalMove(p.getRow(), p.getCol() - 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 1, p.getCol() + 1, p.LegalMove(p.getRow() + 1, p.getCol() + 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 1, p.getCol() + 1, p.LegalMove(p.getRow() - 1, p.getCol() + 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() + 1, p.getCol() - 1, p.LegalMove(p.getRow() + 1, p.getCol() - 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow() - 1, p.getCol() - 1, p.LegalMove(p.getRow() - 1, p.getCol() - 1, ref chessBoard));
                    DisplayPotentialMoves(p.GetName(), p.getRow(), p.getCol() + 2, p.LegalMove(p.getRow(), p.getCol() + 2, ref chessBoard));//Queen Side
                    DisplayPotentialMoves(p.GetName(), p.getRow(), p.getCol() - 2, p.LegalMove(p.getRow(), p.getCol() - 2, ref chessBoard));//Kng Side


                    break;//Its king time

            }

            return Board.Controls.OfType<Control>().ToList().Count - 32;
        }
        private bool SquareBounds(ref Point point)
        {
            //Making Y movement static
            if (point.Y < 45 || point.Y >= 405 || point.X < 45 || point.X >= 405)
            {
                ErrorScreen.Text = "Invalid Position to Move";
                //Console.WriteLine("Invalid Position to Move"); 
                return false;
            }
            else if (point.Y < 90)
                point.Y = 45;
            else if (point.Y < 135)
                point.Y = 90;
            else if (point.Y < 180)
                point.Y = 135;
            else if (point.Y < 225)
                point.Y = 180;
            else if (point.Y < 270)
                point.Y = 225;
            else if (point.Y < 315)
                point.Y = 270;
            else if (point.Y < 360)
                point.Y = 315;
            else if (point.Y < 405)
                point.Y = 360;


            //Making X movement static
            if (point.X < 90)
                point.X = 45;
            else if (point.X < 135)
                point.X = 90;
            else if (point.X < 180)
                point.X = 135;
            else if (point.X < 225)
                point.X = 180;
            else if (point.X < 270)
                point.X = 225;
            else if (point.X < 315)
                point.X = 270;
            else if (point.X < 360)
                point.X = 315;
            else if (point.X < 360)
                point.X = 315;
            else if (point.X < 405)
                point.X = 360;
            return true;
        }

        private void CleanUpMoves()
        {
            foreach (Control item in Board.Controls.OfType<Control>().ToList())
                if (item.Tag == Board)
                    Board.Controls.Remove(item);
            foreach (Pieces p in attacked)
                p.GetImage().BackColor = Color.Transparent;
            attacked.Clear();
        }

        private void Castling(object sender, EventArgs e)
        {
            //moving the rook
            Point newPos = ((PictureBox)sender).Location;
            int newBoardPos = newPos.X / 45 - 1 + 8 * (newPos.Y / 45 - 1);
            int oldBoardPos = prevSelected.Location.X / 45 - 1 + 8 * (prevSelected.Location.Y / 45 - 1);
            if (newBoardPos > oldBoardPos)//king side
            {
                Swap(oldBoardPos + 3, newBoardPos - 1);
                chessBoard[newBoardPos - 1].setRow(newPos.Y / 45);
                chessBoard[newBoardPos - 1].setCol(newPos.X / 45 - 1);
            }
            else//Queen Side
            { 
                Swap(oldBoardPos - 4, newBoardPos + 1);
                chessBoard[newBoardPos + 1].setRow(newPos.Y / 45);
                chessBoard[newBoardPos + 1].setCol(newPos.X / 45 + 1);
            }

            MovePiece(sender, e);

        }
        private void DisplayPotentialMoves(char n, int r, int c, int CaptureOrMove)
        {
            switch (CaptureOrMove)
            {
                case 0://Normal Move
                    PictureBox possiblePos = new PictureBox
                    {
                        Name = n + c.ToString() + r.ToString(),
                        Parent = Board,
                        BackColor = Color.LightYellow,
                        Location = new Point(c * 45, r * 45),
                        Size = new Size(45, 45),
                        BorderStyle = BorderStyle.FixedSingle,
                        Tag = Board
                    };
                    possiblePos.Click += new EventHandler(MovePiece);
                    Board.Controls.Add(possiblePos);
                    break;

                case 1://Capture
                    attacked.Add(chessBoard[8 * (r - 1) + c - 1]);
                    attacked.Last().GetImage().BackColor = Color.LightCoral;
                    break;

                case 2:
                case -2:
                    PictureBox castlingPos = new PictureBox
                    {
                        Name = n + c.ToString() + r.ToString(),
                        Parent = Board,
                        BackColor = Color.LightBlue,
                        Location = new Point(c * 45, r * 45),
                        Size = new Size(45, 45),
                        BorderStyle = BorderStyle.FixedSingle,
                        Tag = Board
                    };
                    castlingPos.Click += new EventHandler(Castling);
                    Board.Controls.Add(castlingPos);
                    break;

                default:
                    break;
            }
        }
    }
}