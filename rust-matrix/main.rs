
use std::io;

fn main() {
    let mut input = String::new();

    println!("행의 개수를 입력하세요:");
    
    io::stdin()
        .read_line(&mut input)
        .expect("입력 실패");

    println!("입력한 값: {}", input.trim());

    let rows: usize = input.trim().parse().expect("숫자를 입력하세요"); //rust는 입력받은 값이 문자열이라서 숫자로 변환해줘야함

    input.clear(); // input 초기화

    println!("열의 개수를 입력하세요:");

    io::stdin()
        .read_line(&mut input)
        .expect("입력 실패");
    
    let cols : usize = input.trim().parse().expect("숫자를 입력하세요");


    let mut a = vec![
        vec![0; cols];
        rows
    ]; //vec![채울 값; 개수] 

    let mut b = vec![
        vec![0; cols];
        rows
    ]; //vec![채울 값; 개수] 

    let mut result = vec![
        vec![0; cols];
        rows
    ]; //vec![채울 값; 개수] 

    println!("생성된 A행렬:");

    for i in 0..rows {
        for j in 0..cols {
            a[i][j] = rand::random_range(1..=9); // 1~9 사이의 랜덤한 값으로 초기화
            print!("{:>3}", a[i][j]);
        }
        println!();
    }

    println!("생성된 B행렬:");

    for i in 0..rows {
        for j in 0..cols {
            b[i][j] = rand::random_range(1..=9); // 1~9 사이의 랜덤한 값으로 초기화
            print!("{:>3 }", b[i][j]);
        }
        println!();
    }

    println!("결과 행렬:");

    for i in 0..rows {
        for j in 0..cols {
            result[i][j] = a[i][j] + b[i][j];
            print!("{:>3}", result[i][j]);
        }
        println!();
    }

}