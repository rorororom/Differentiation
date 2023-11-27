\documentclass{article}
\usepackage[a4paper]{geometry}
\geometry{top=1.41cm, bottom=1.41cm, left=1.41cm, right=1.41cm, marginparwidth=1.75cm}
\usepackage[T2A]{fontenc}
\usepackage[utf8]{inputenc}
\usepackage[english, russian]{babel}
\usepackage{amsmath}
\usepackage{graphicx}
\usepackage[colorlinks=true, allcolors=blue]{hyperref}
\usepackage{amsfonts}
\usepackage{amssymb}
\usepackage{seqsplit}
\usepackage[dvipsnames]{xcolor}
\usepackage{enumitem}
\usepackage{algorithm}
\usepackage{algpseudocode}
\usepackage{algorithmicx}
\usepackage{mathalfa}
\usepackage{mathrsfs}
\usepackage{dsfont}
\usepackage{caption,subcaption}
\usepackage{wrapfig}
\usepackage[stable]{footmisc}
\usepackage{indentfirst}
\usepackage{rotating}
\usepackage{pdflscape}
\usepackage{MnSymbol,wasysym}
\begin{document}
$\text{После некоторых методов диференцирования функции мы получили математическое выражение:}$
$$ \frac { \frac { sin( x ) } { cos( x ) } } { x ^ { 2 } } $$
$\text{Легко заметить, что:}$
$$ \frac { \frac { cos( x ) \cdot 1 \cdot cos( x ) - sin( x ) \cdot sin( x ) \cdot -1 } { ( cos( x ) ) ^ { 2 } } \cdot x ^ { 2 } - \frac { sin( x ) } { cos( x ) } \cdot 2 \cdot x ^ { 2 - 1 } \cdot 1 } { ( x ^ { 2 } ) ^ { 2 } } $$
$\text{Производная этой части выражена явно через следующее математическое выражение:}$
$$ \frac { \frac { cos( x ) \cdot cos( x ) - sin( x ) \cdot sin( x ) \cdot -1 } { ( cos( x ) ) ^ { 2 } } \cdot x ^ { 2 } - \frac { sin( x ) } { cos( x ) } \cdot 2 \cdot x ^ { 2 - 1 } \cdot 1 } { ( x ^ { 2 } ) ^ { 2 } } $$
$\text{Производная этой части выражена явно через следующее математическое выражение:}$
$$ \frac { \frac { cos( x ) \cdot cos( x ) - sin( x ) \cdot sin( x ) \cdot -1 } { ( cos( x ) ) ^ { 2 } } \cdot x ^ { 2 } - \frac { sin( x ) } { cos( x ) } \cdot 2 \cdot x ^ { 2 - 1 } } { ( x ^ { 2 } ) ^ { 2 } } $$
$\text{Производная этой части выражена явно через следующее математическое выражение:}$
$$ \frac { \frac { cos( x ) \cdot cos( x ) - sin( x ) \cdot sin( x ) \cdot -1 } { ( cos( x ) ) ^ { 2 } } \cdot x ^ { 2 } - \frac { sin( x ) } { cos( x ) } \cdot 2 \cdot x ^ { 1 } } { ( x ^ { 2 } ) ^ { 2 } } $$
$\text{Когда производная говорит 'прыгни', не спрашивай 'почему', просто прыгай!}$
$$ \frac { \frac { cos( x ) \cdot cos( x ) - sin( x ) \cdot sin( x ) \cdot -1 } { ( cos( x ) ) ^ { 2 } } \cdot x ^ { 2 } - \frac { sin( x ) } { cos( x ) } \cdot 2 \cdot x } { ( x ^ { 2 } ) ^ { 2 } } $$
\end{document}
