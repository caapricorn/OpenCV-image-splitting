while :; do
    sleep 60
    git checkout auto
    git fetch origin
    git pull
    git add .
    git commit -m "autocommit-$(date)"
    git push
done
