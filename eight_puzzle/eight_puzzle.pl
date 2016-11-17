
% The Eight-Puzzle
% by Philip Westrich and Mitchell White
% for the CSC 4240 Honors Assignment
% Fall 2014
%
% This program solves the Eight-Puzzle, something that should have been 
% discussed in class. It uses the general purpouse iterative-deepening 
% planner from the book "Thinking and Computation". I saw a set of rules 
% for a 2x3 version of the puzzle; using them as a base, I wrote the ones
% needed for a 3x3 puzzle. It can actually be easily adapted for different 
% sizes by editing the legal_move rules.
%
% To use the program, set the initial and final states to what you want,
% load the program, and type "solve(L).".

% This looks for plans, short ones first, using the plan predicate.
% It's an iterative-deepening style method.
% solve(L) holds if L is a plan.
solve(L) :- tryplan([],L). 

% tryplan(X,L): L is a plan and has X as its final elements.
tryplan(L,L) :- plan(L).
tryplan(X,L) :- tryplan([_|X],L).

% This general planner needs the predicates below to be defined:
%     - legal_move(BeforeState,Move,AfterState)
%     - initial_state(State)
%     - goal_state(State)
% There are probably much faster ways to do this.
% plan(L): L is a list of moves from the initial state to a goal state.
plan(L) :- initial_state(I), goal_state(G), reachable(I,L,G).

% reachable(S1,L,S2): S2 is reachable from S1 using moves L.
reachable(S,[],S).           
reachable(S1,[M|L],S3) :- legal_move(S1,M,S2), reachable(S2,L,S3).

% The goal state. 0 is the blank tile.
% Each space-seperated group is a row of the puzzle, from top to bottom.
goal_state([1,2,3, 4,5,6, 7,8,0]).

% The initial state, something random
% Some states are unsolvable. Here are a few that are. 
%initial_state([4,1,6, 7,5,3, 2,8,0]).
%initial_state([4,7,6, 2,5,3, 0,1,8]).
%initial_state([5,2,1, 7,0,8, 3,6,4]).
initial_state([1,2,3, 0,7,6, 5,4,9]).

% these are legal left moves, X is the moving tile, 0 is the blank
legal_move([0,X,C, D,E,F, G,H,I],left(X),[X,0,C, D,E,F, G,H,I]). 
legal_move([A,0,X, D,E,F, G,H,I],left(X),[A,X,0, D,E,F, G,H,I]). 
legal_move([A,B,C, 0,X,F, G,H,I],left(X),[A,B,C, X,0,F, G,H,I]). 
legal_move([A,B,C, D,0,X, G,H,I],left(X),[A,B,C, D,X,0, G,H,I]). 
legal_move([A,B,C, D,E,F, 0,X,I],left(X),[A,B,C, D,E,F, X,0,I]).
legal_move([A,B,C, D,E,F, G,0,X],left(X),[A,B,C, D,E,F, G,X,0]).

% right moves are the opposite of left moves
legal_move(S1,right(X),S2) :- legal_move(S2,left(X),S1).

% these are legal up moves
legal_move([0,B,C, X,E,F, G,H,I],up(X),[X,B,C, 0,E,F, G,H,I]).
legal_move([A,0,C, D,X,F, G,H,I],up(X),[A,X,C, D,0,F, G,H,I]).
legal_move([A,B,0, D,E,X, G,H,I],up(X),[A,B,X, D,E,0, G,H,I]).
legal_move([A,B,C, 0,E,F, X,H,I],up(X),[A,B,C, X,E,F, 0,H,I]). 
legal_move([A,B,C, D,0,F, G,X,I],up(X),[A,B,C, D,X,F, G,0,I]).
legal_move([A,B,C, D,E,0, G,H,X],up(X),[A,B,C, D,E,X, G,H,0]).

% down moves are the opposite of up moves
legal_move(S1,down(X),S2) :- legal_move(S2,up(X),S1).
