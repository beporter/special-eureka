use std::fs::read_dir;
use std::path::Path;
use std::env;
use std::process::ExitCode;

fn dir_size(dirname: &Path) -> Result<u64, &'static str> {
  let mut sum: u64 = 0;

  if let Ok(entries) = read_dir(dirname) {
    for entry in entries {
      let entry = entry.unwrap();
      let path = entry.path();

      if path.is_dir() && !entry.file_name().into_string().unwrap().starts_with(".") {
        eprintln!("dir_size(): Recursing into dir. ({})", path.display());
        sum += dir_size(&path).unwrap();
      } else {
        if let Ok(metadata) = entry.metadata() {
          eprintln!("dir_size(): File size ({}): ({})",
            path.display(),
            metadata.len(),
          );
          sum += metadata.len();
        }

      }
    }
  }

  return Ok(sum);
}


fn main() -> ExitCode {

  println!("Path size scanner.");

  let args: Vec<String> = env::args().collect();
  // dbg!(args);
  if args.len() < 2 {
    eprintln!("Provide a filesystem path as first argument.");
    std::process::exit(1);
  }

  let file_path = &args[1];

  if !Path::new(file_path).is_dir() {
    eprintln!("Not a valid directory: {}", file_path);
    std::process::exit(3);
  }

  println!("Scanning {}", file_path);
  let result = dir_size(Path::new(file_path));

  match result {
    Ok(_) => {
      println!("Done. {}", result.unwrap());
      std::process::exit(0);
    }
    Err(e) => {
      eprintln!("Failed: {}", e);
      std::process::exit(2);
    }
  }
}
