mkdir -p directory
cat << EOF > input.txt
Hello, World

This is a test file to check student shell programs.

Thanks for your time!
EOF

cat << EOF > input2.txt
Hello world 2, this is a secondary file
that was created to test some input scenarios
EOF

cat <<EOF > append.txt
Appending some text to
the 
file!
EOF

cat <<EOF > output.txt
EOF
printf "[nyush grading]$ " > refout/refoutput26
cat refout/refoutput26 > refout/refoutput12
ls -f /bin >> refout/refoutput26
ls -f / >> refout/refoutput12
echo "[nyush grading]$ " >> refout/refoutput26
echo "[nyush grading]$ " >> refout/refoutput12

