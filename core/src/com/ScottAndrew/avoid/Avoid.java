package com.ScottAndrew.avoid;

import java.util.Iterator;
import java.util.Random;

import com.badlogic.gdx.Application;
import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input.Keys;
import com.badlogic.gdx.audio.Music;
import com.badlogic.gdx.audio.Sound;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.graphics.g2d.TextureRegion;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer;
import com.badlogic.gdx.graphics.glutils.ShapeRenderer.ShapeType;
import com.badlogic.gdx.math.Circle;
import com.badlogic.gdx.math.MathUtils;
import com.badlogic.gdx.math.Polygon;
import com.badlogic.gdx.math.Rectangle;
import com.badlogic.gdx.math.Vector3;
import com.badlogic.gdx.scenes.scene2d.InputEvent;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Dialog;
import com.badlogic.gdx.scenes.scene2d.ui.Skin;
import com.badlogic.gdx.scenes.scene2d.ui.TextButton;
import com.badlogic.gdx.scenes.scene2d.utils.ClickListener;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.TimeUtils;


public class Avoid extends ApplicationAdapter {
	public final int WIN_WIDTH = 1920;
	public final int WIN_HEIGHT= 1080;


	private OrthographicCamera camera;
	private SpriteBatch batch;
	private ShapeRenderer divider;
	private long lastDropTime;
	private Array<Rectangle> shapes;
	private Sound dying;
//	private Music backMusic;
	private Rectangle leftThumb;
	private Rectangle rightThumb;
	private Texture thumbImage;
	private Texture thumbImage2;

	//Popup Box When Lost
	private Stage stage;
	private Skin skin;
	private TextButton yes;
	private TextButton no;

	@Override
	public void create () {
		//Creates the Texture
		thumbImage = new Texture(Gdx.files.internal("Square.png"));
		thumbImage2 = new Texture(Gdx.files.internal("Square.png"));

		//Camera
		camera = new OrthographicCamera();
		camera.setToOrtho(false, WIN_WIDTH, WIN_HEIGHT);
		batch = new SpriteBatch();

		//Thumb Indicators
		leftThumb = new Rectangle();
		leftThumb.height  = 100;
		leftThumb.width = 100;
		leftThumb.x = WIN_WIDTH/4 - 50;
		leftThumb.y = WIN_HEIGHT/2 - 50;
		rightThumb = new Rectangle();
		rightThumb.height = 100;
		rightThumb.width = 100;
		rightThumb.x = (WIN_WIDTH/4)*3 - 50;
		rightThumb.y = WIN_HEIGHT/2 - 50;


		//Divider
		divider = new ShapeRenderer();
		shapes = new Array<Rectangle>();

		//Sounds
		dying = Gdx.audio.newSound(Gdx.files.internal("Boop.wav"));
//		backMusic = Gdx.audio.newMusic(Gdx.files.internal("Epicstep.mp3"));
//		backMusic.setLooping(true);
//		backMusic.play();

	}

	@Override
	public void render () {
		renderBackground();
		camera.update();
		batch.setProjectionMatrix(camera.combined);
		batch.begin();
		//batch.draw(bucketImage, bucket.x, bucket.y);

		batch.draw(thumbImage, leftThumb.x, leftThumb.y);
		batch.end();
		batch.begin();
		batch.draw(thumbImage2, rightThumb.x, rightThumb.y);
		batch.end();
		batch.begin();


		//Controlling right thumb
		if(Gdx.input.isKeyPressed(Keys.LEFT)) rightThumb.x -= 600 * Gdx.graphics.getDeltaTime();
		if(Gdx.input.isKeyPressed(Keys.RIGHT)) rightThumb.x += 600 * Gdx.graphics.getDeltaTime();
		if(Gdx.input.isKeyPressed(Keys.DOWN)) rightThumb.y -= 600 * Gdx.graphics.getDeltaTime();
		if(Gdx.input.isKeyPressed(Keys.UP)) rightThumb.y += 600 * Gdx.graphics.getDeltaTime();


		//conrolling left thumb
		if(Gdx.input.isKeyPressed(Keys.A)) leftThumb.x -= 600 * Gdx.graphics.getDeltaTime();
		if(Gdx.input.isKeyPressed(Keys.D)) leftThumb.x += 600 * Gdx.graphics.getDeltaTime();
		if(Gdx.input.isKeyPressed(Keys.S)) leftThumb.y -= 600 * Gdx.graphics.getDeltaTime();
		if(Gdx.input.isKeyPressed(Keys.W)) leftThumb.y += 600 * Gdx.graphics.getDeltaTime();

		//Touch Controls
		if(Gdx.app.getType() == Application.ApplicationType.Android){
			Vector3 touchPos = new Vector3();
			Vector3 touchPos2 = new Vector3();
			for(int i = 0; i <2; i++){
				int touchedX = Gdx.input.getX(i);
				if(touchedX > WIN_WIDTH/2){
					touchPos2 = new Vector3();
					touchPos2.set(Gdx.input.getX(i), Gdx.input.getY(i), 0);
					camera.unproject(touchPos2.set(Gdx.input.getX(i),Gdx.input.getY(i),0));
					rightThumb.x = touchPos2.x - 50;
					rightThumb.y = touchPos2.y - 50;
				}else{
					touchPos = new Vector3();
					touchPos.set(Gdx.input.getX(i), Gdx.input.getY(i), 0);
					camera.unproject(touchPos.set(Gdx.input.getX(i),Gdx.input.getY(i),0));
					leftThumb.x = touchPos.x - 50;
					leftThumb.y = touchPos.y - 50;
				}
			}
		}



		//Bounds the right Circle from escaping the window
		if(rightThumb.x < WIN_WIDTH/2) rightThumb.x = WIN_WIDTH/2;
		if(rightThumb.x > WIN_WIDTH - 100) rightThumb.x = WIN_WIDTH - 100;
		if(rightThumb.y < 0) rightThumb.y = 0;
		if(rightThumb.y > WIN_HEIGHT-100) rightThumb.y = WIN_HEIGHT-100;

		//Bounds the left Circle
		if(leftThumb.x < 0) leftThumb.x = 0;
		if(leftThumb.x > WIN_WIDTH/2 -100) leftThumb.x = WIN_WIDTH/2 -100;
		if(leftThumb.y < 0) leftThumb.y = 0;
		if(leftThumb.y > WIN_HEIGHT-100) leftThumb.y = WIN_HEIGHT-100;



		if(TimeUtils.nanoTime() - lastDropTime > 1000000000)spawnHitbox(); //1000000000/4 = quarter second
		Iterator<Rectangle> it = shapes.iterator();
		while(it.hasNext()){
			Rectangle p = it.next();
			p.y -=400 * Gdx.graphics.getDeltaTime();
			if(p.overlaps(leftThumb) || p.overlaps(rightThumb)) {
				dying.play();
				it.remove();
				//tryAgain();
			}
			if(p.y < -1*p.height) it.remove();
		}

		ShapeRenderer sr = new ShapeRenderer();
		sr.begin(ShapeType.Filled);
		sr.setProjectionMatrix(camera.combined);
		for(Rectangle p: shapes){
			//TextureRegion region = new TextureRegion(new Texture(Gdx.files.internal("Square.png")), (int)p.width, (int)p.height);
//			Texture region = new Texture(Gdx.files.internal("Square.png"));
//			batch.draw(region, p.x, p.y);
			sr.setColor(1, 0, 0, 1);
			sr.rect(p.x, p.y, p.width, p.height);
		}
		sr.end();

		divider.begin(ShapeType.Line);
		divider.setColor(1, 1, 1, 1);
		divider.line(WIN_WIDTH/2, 0, WIN_WIDTH/2, WIN_HEIGHT);
		divider.end();
		batch.end();
	}

	public void renderBackground(){
		//draw background
		Gdx.gl.glClearColor(0, 0, 0, 0);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);

		//render dividing middle line
		divider.begin(ShapeType.Line);
		divider.setColor(1, 1, 1, 1);
		divider.line(WIN_WIDTH/2, 0, WIN_WIDTH/2, WIN_HEIGHT);
		divider.end();



	}

	public void spawnHitbox(){
		float x = (float)MathUtils.random(WIN_WIDTH/2);
		float y = (float)WIN_HEIGHT;
		float width = (float)(MathUtils.random(WIN_WIDTH/4)*.66); width = width<100?100:width;
		float height = (float)MathUtils.random(WIN_HEIGHT/3); height = height<100?100:height;

		
		shapes.add(new Rectangle(x, y, width, height));
		
		x = (float)MathUtils.random(WIN_WIDTH/2)+WIN_WIDTH/2;
		y = (float)WIN_HEIGHT;
		width = (float)(MathUtils.random(WIN_WIDTH/4)*.66); width = width<100?100:width;
		height = (float)MathUtils.random(WIN_HEIGHT/3); height = height<100?100:height;
		
		shapes.add(new Rectangle(x, y, width, height));
		lastDropTime = TimeUtils.nanoTime();
	}

	public void tryAgain() {
		Gdx.input.setInputProcessor(stage = new Stage());
		skin = new Skin(Gdx.files.internal("uiskin.json"));

		new Dialog("confirm exit", skin) {

			{
				text("rly exit");
				button("yes", "goodbye");
				button("no", "glad you stay");
			}

			@Override
			protected void result(final Object object) {
				new Dialog("", skin) {

					{
						text(object.toString());
						button("OK");
					}

				}.show(stage);
			}

		}.show(stage);
	}



	public void render(float delta) {
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		stage.act(delta);
		stage.draw();
	}

	public void hide() {
		dispose();
	}

	@Override
	public void dispose() {
		thumbImage.dispose();
		thumbImage2.dispose();
//		backMusic.dispose();
		dying.dispose();
		batch.dispose();
		stage.dispose();
		skin.dispose();
	}

}
