while :; do
    sleep 5
    git checkout auto
    git fetch origin
    git pull
    git add .
    git commit -m "autocommit-$(date)"
    git push
done
