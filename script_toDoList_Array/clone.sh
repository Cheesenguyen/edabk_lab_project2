repo_url=$1

git clone $repo_url

repo_name=$(basename $repo_url)
repo_name=${repo_name%.git}
echo $repo_name

cd $repo_name

echo "Đã clone thành công và chuyển vào thư mục: $repo_name"