use std::io;
use std::cmp;

fn main() {
    let mut input: String = String::new();//empty string
    
    io::stdin().read_line(&mut input).expect("Fxxk");
    let val: Vec<i32> = input.trim().split_whitespace()
        .map(|x| x.parse::<i32>().unwrap())
        .collect::<Vec<_>>();
    let ans: i32 = cmp::max(val[0] * val[1] / val[2], val[0] / val[1] * val[2]);
    //println!("{}", ans);
}