import os
import asyncio
import json
from playwright.async_api import async_playwright
from markitdown import MarkItDown

md = MarkItDown()
output_dir = "TPT_Markdown_English"
SNAP_CHROMIUM_PATH = "/snap/bin/chromium" 

os.makedirs(output_dir, exist_ok=True)

async def convert_wiki():
    async with async_playwright() as p:
        browser = await p.chromium.launch(executable_path=SNAP_CHROMIUM_PATH, headless=False)
        context = await browser.new_context(
            user_agent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36"
        )
        page = await context.new_page()

        print("Navigating to Wiki to clear Anubis...")
        # Go to the main page first to solve any initial bot checks
        await page.goto("https://powdertoy.co.uk/Wiki/W/Main_Page.html", wait_until="networkidle")
        
        # If you see a challenge, solve it manually or wait for the script to continue
        print("Fetching page list via API...")
        api_url = "https://powdertoy.co.uk/Wiki/api.php?action=query&list=allpages&aplimit=max&format=json"
        
        response = await page.goto(api_url)
        
        try:
            # Try to parse the response as JSON directly
            data = await response.json()
        except Exception:
            # If that fails, it's likely showing a landing page or error
            print("API did not return JSON. You might need to solve a captcha in the browser window.")
            # Wait for you to solve it manually, then it tries to grab the text
            await asyncio.sleep(10) 
            raw_json = await page.evaluate("document.body.innerText")
            data = json.loads(raw_json)

        all_pages = [p['title'] for p in data['query']['allpages']]
        en_pages = [p for p in all_pages if not (len(p) > 3 and p[-3] == '/' and p[-2:].islower())]

        print(f"Found {len(en_pages)} English pages.")

        for title in en_pages:
            url_title = title.replace(" ", "_")
            url = f"https://powdertoy.co.uk/Wiki/W/{url_title}.html"
            
            try:
                print(f"Converting: {title}")
                await page.goto(url, wait_until="networkidle")
                
                html_content = await page.content()
                
                # Using a fixed temp file name to avoid clutter
                temp_file = "current_page.html"
                with open(temp_file, "w", encoding="utf-8") as f:
                    f.write(html_content)
                
                result = md.convert(temp_file)
                
                safe_name = url_title.replace("/", "_") + ".md"
                with open(os.path.join(output_dir, safe_name), "w", encoding="utf-8") as f:
                    f.write(result.text_content)
                
                await asyncio.sleep(1) # Be nice
                
            except Exception as e:
                print(f"Error on {title}: {e}")

        await browser.close()

if __name__ == "__main__":
    asyncio.run(convert_wiki())