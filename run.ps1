$args = @("log_{e}(e^{x})=x","F_{21}^{3}=2^{5}*7^{3}-30","sin^{3}(\frac{1}{3}\pi)=\frac{3}{8}\sqrt{3}","x=\frac{-b+\sqrt{b^{2}-4ac}}{2a}","x=\frac{\root{3}{-2b^{3}+9abc-27a^{2}d+\sqrt{4(-b^{2}+3ac)^{3}+(-2b^{3}+9abc-27a^{2}d)^{2}}}}{3\root{3}{2}a} - \frac{b}{3a} - \frac{\root{3}{2}(-b^{2}+3ac)}{3a\root{3}{-2b^{3}+9abc-27a^{2}d+\sqrt{4(-b^{2}+3ac)^{3}+(-2b^{3}+9abc-27a^{2}d)^{2}}}}")
foreach ($arg in $args)
{
    "Input: " + $arg
    "Output: "
    " "
    .\main.exe $arg
    
    " "
    " "
}